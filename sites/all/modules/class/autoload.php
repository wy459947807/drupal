<?php

function classLoader($class) {
    $path = str_replace('\\', DIRECTORY_SEPARATOR, $class);
    $file = __DIR__ . '\\' . $path . '.php';
    if (file_exists($file)) {
        require_once $file;
    }
}

function autoload_file($file_type) {
    $handle = opendir(__DIR__);
    if ($handle) {
        while (($fl = readdir($handle)) !== false) {
            $temp = __DIR__ . DIRECTORY_SEPARATOR . $fl;
            //如果不加  $fl!='.' && $fl != '..'  则会造成把$dir的父级目录也读取出来
            if (is_dir($temp) && $fl != '.' && $fl != '..') {
                // echo '目录：'.$temp.'<br>';
                autoload_file($temp);
            } else {
                if ($fl != '.' && $fl != '..') {
                    if (pathinfo($temp, PATHINFO_EXTENSION) == $file_type) {
                        include_once $temp;
                    }
                }
            }
        }
    }
}


spl_autoload_register('classLoader');//自动加载类文件
autoload_file("inc");//自动加载inc文件
