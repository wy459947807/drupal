<?php

function app_annotate_list() {

    $result = array(
        'status' =>$_POST,
        'message' => $_GET,
        'data' => common_receive_data(),
    );
    //$result=module_invoke("common","common_api_test");
    //$result=common_test();
    
    //echo "<pre>";
    //var_dump(sql_annotate_article_list("okok"));
    
   
    echo json_encode($result);
}
