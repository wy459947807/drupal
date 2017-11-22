<?php



function web_annotate_list() {
  
    //global $theme;
    //&theme="Bartik";

    //echo "<pre>";
    //var_dump($GLOBALS);
   
    $params=array(
        "sqlFrom"=>" dp_variable ",
        "sqlWhere"=>" name=:name ",
        "bindValues"=>array(
            ":name"=>"admin_theme"
        )
    );
    
    $dataInfo= sql_getPageList($params);
    echo "<pre>";
    echo var_dump($dataInfo);
    exit;
    
    
    $data=array("id"=>"459947807");
    $rules = array(
        array('id','email','id不得为空！',1,'regex',3),
    );
    common_validate($data,$rules);
    exit;
    

    $dataInfo = json_decode(common_remote_data("/app/annotate/annotate_list",array("test"=>888)),true);
    //$dataInfo = common_remote_data("http://www.baidu.com",array());
   /* echo "<pre>";
    var_dump($dataInfo);
    exit;*/
   
    
    $dataInfo=array();
    $dataInfo['test']="OK";
    //var_dump($dataInfo);

    //echo drupal_get_path('module', 'web_annotate');
    //echo current_path();
    //echo base_path();
    //echo request_uri();
    //echo $base_root;
    //exit;
    $output = theme('web_annotate_wrapper', array('dataInfo' =>$dataInfo));
    //$output.="666777";
    //echo json_encode($result);
    //return $output;
    return $output;
}
