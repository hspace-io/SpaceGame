<?php

if( isset($_GET['mode']) && $_GET['mode'] == 'base64' && isset($_GET['image']) ){

	if(preg_match("/flag/is", $_GET['image'])){ die("no hack!"); }

        if( preg_match("/\//is", $_GET['image']) ){
                $content = file_get_contents($_GET['image']);
        }else{
                $content = file_get_contents("/tmp/".$_GET['image']);
	}
	if( isset($_GET['raw']) ){ echo $content; }
	else{ echo base64_encode($content); }
}else{ ?>
<html>
    <head>
        <?php include_once './templates/head.html'; ?>
                <style>
                        .main{ text-align:center; }
                        .tree{
                                display: inline-block;
                                width: 60%;
                                text-align: left;
                        }
                        h1{ margin-top: 20px; margin-bottom: 30px; }
                </style>
    </head>
    <body>
        <?php include_once './templates/nav.html'; ?>
                <div class="main">
                <div class="tree">
                                <h1>FILE Explore</h1>
                                <ul>
                                        <?php
                                                if ($handle = opendir('/tmp/')) {
                                                        while (false !== ($entry = readdir($handle))) {
                                                                if ($entry != "." && $entry != ".." ) {
                                                                                echo "<li><a href='/images/$entry'>$entry</a> <a href='?mode=base64&image=$entry'>[base64]</a></li>\n";
                                                                        }
                                                                }
                                                                closedir($handle);
                                                }
                                        ?>
                                </ul>
                        </div>
                </div>
    </body>
</html><?php } ?>
