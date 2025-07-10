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
