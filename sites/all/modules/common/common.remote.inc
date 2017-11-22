<?php

/**
 * POST,GET,PUT请求远程页面数据
 */
function common_remote_data($url, $data, $type = "POST", $content = "data") {

    global $base_url;

    if (strpos($url, '://') == FALSE) {
        $url = $base_url . "/" . $url;
    }

    switch ($type) {
        case "POST":
            $data = drupal_http_build_query($data);
            $options = array(
                'method' => $type,
                'data' => $data,
                'timeout' => 15,
                'headers' => array('Content-Type' => 'application/x-www-form-urlencoded'),
            );
            $dataInfo = drupal_http_request($url, $options);
            break;
        case "GET":
            $url = url($url, array('query' => $data));
            $dataInfo = drupal_http_request($url);
            break;
        case "PUT":
            $data = json_encode($data);
            $options = array(
                'method' => $type,
                'data' => $data,
                'timeout' => 15,
                'headers' => array('Content-Type' => 'text/xml;charset=UTF-8'),
            );
            $dataInfo = drupal_http_request($url, $options);
            break;
    }
    return $dataInfo->$content;
}

/**
 * 接收远程数据
 */
function common_receive_data() {
    switch ($_SERVER['REQUEST_METHOD']) {
        case 'POST':
            $input = $_POST;
            break;
        case 'PUT':
            $input = json_decode(file_get_contents('php://input'), TRUE);
            break;
        default:
            $input = $_GET;
    }
    return $input;
}

/**
 * 页面返回json数据
 */
function common_json_return($status = 200, $msg = '', $data = '') {
    $data=!empty($data)?$data:null;
    common_data_return(array(
        'status' => $status,
        'message' => $msg,
        'data' => $data,
    ));
}

/**
 * 页面返回数据
 */

/**
 * Ajax方式返回数据到客户端
 * @access protected
 * @param mixed $data 要返回的数据
 * @param String $type AJAX返回数据格式
 * @return void
 */
function common_data_return($data, $type = 'JSON', $json_option = 0) {
    switch (strtoupper($type)) {
        case 'JSON' :
            //跨域处理
            header('Access-Control-Allow-Origin: *');
            header("Access-Control-Allow-Headers: Origin, X-Requested-With, Content-Type, Accept");
            header('Access-Control-Allow-Methods: GET, POST, PUT');

            // 返回JSON数据格式到客户端 包含状态信息
            header('Content-Type:application/json; charset=utf-8');
            exit(json_encode($data, $json_option));
        case 'XML' :
            // 返回xml格式数据
            header('Content-Type:text/xml; charset=utf-8');
            exit(xml_encode($data));
        case 'JSONP':
            // 返回JSON数据格式到客户端 包含状态信息
            header('Content-Type:application/json; charset=utf-8');
            $handler = isset($_GET["callback"]) ? $_GET["callback"] : "jsonpReturn";
            exit($handler . '(' . json_encode($data, $json_option) . ');');
        case 'EVAL' :
            // 返回可执行的js脚本
            header('Content-Type:text/html; charset=utf-8');
            exit($data);
        case 'AJAX_UPLOAD':
            // 返回JSON数据格式到客户端 包含状态信息
            header('Content-Type:text/html; charset=utf-8');
            exit(json_encode($data, $json_option));
        default :
            break;
    }
}
