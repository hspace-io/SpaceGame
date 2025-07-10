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
