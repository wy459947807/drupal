<?php


/**
 * 参数验证页面跳转执行函数
 */
function common_validate($data,$rules,$errorPage){
    $result=common_validate_data($data,$rules);
    if($result['status']==500){
        if(!empty($errorPage)){
            drupal_goto($errorPage,array("query"=>array("message"=>$result['message'])));
        }else{
            common_json_return($result['status'],$result['message'],$result['data']);
        }
    }    
}


/**
 * 数据验证
 */
function common_validate_data($data,$rules){
    $validateInfo = array('status' =>200,'message' => "成功！",'data' => array());
    $validateResult=TRUE; 
    foreach($rules as $key=>$val) {
        // 验证因子定义格式
        // array(field,rule,message,condition,type,when,params)
        // 判断是否需要执行验证
        if(empty($val[5]) || ( $val[5]== 3 && $type < 3 ) || $val[5]== $type ) {
            $val[3]  =  isset($val[3])?$val[3]:0;
            $val[4]  =  isset($val[4])?$val[4]:'regex';
            // 判断验证条件
            switch($val[3]) {
                case 0:   // 必须验证 不管表单是否有设置该字段
                    if(false === common_validate_Item($data,$val)){ 
                        $validateResult=FALSE;
                    }
                    break;
                case 2:    // 值不为空的时候才验证
                    if('' != trim($data[$val[0]])){
                        if(false === common_validate_Item($data,$val)){ 
                             $validateResult=FALSE;
                        }
                    }
                    break;
                default:    // 默认表单存在该字段就验证
                    if(isset($data[$val[0]])){
                        if(false === common_validate_Item($data,$val)){ 
                            $validateResult=FALSE;
                        }
                    }
            }
        }
        
        if(!$validateResult){
            $validateInfo['status']=500;
            $validateInfo['message']=$val[2];
            $validateInfo['data']=array("error_field"=>$val[0],"error_type"=>$val[1]);
            return $validateInfo;
        }
    }
    
    return $validateInfo;
    //foreach ($data)
    
}


/**
 * 根据验证因子验证字段
 * @access protected
 * @param array $data 创建数据
 * @param array $val 验证因子
 * @return boolean
 */
function common_validate_Item($data,$val) {
   switch(strtolower(trim($val[4]))) {
       case 'function':// 使用函数进行验证
       case 'callback':// 调用方法进行验证
           $args = isset($val[6])?(array)$val[6]:array();
           if(is_string($val[0]) && strpos($val[0], ','))
               $val[0] = explode(',', $val[0]);
           if(is_array($val[0])){
               // 支持多个字段验证
               foreach($val[0] as $field)
                   $_data[$field] = $data[$field];
               array_unshift($args, $_data);
           }else{
               array_unshift($args, $data[$val[0]]);
           }
           if('function'==$val[4]) {
               return call_user_func_array($val[1], $args);
           }else{
               return call_user_func_array(array(&$this, $val[1]), $args);
           }
       case 'confirm': // 验证两个字段是否相同
           return $data[$val[0]] == $data[$val[1]];
       default:  // 检查附加规则
           return common_check($data[$val[0]],$val[1],$val[4]);
   }
}



/**
 * 使用正则验证数据
 * @access public
 * @param string $value  要验证的数据
 * @param string $rule 验证规则
 * @return boolean
 */
function common_regex($value,$rule) {
   $validate = array(
       'require'   =>  '/\S+/',
       'email'     =>  '/^\w+([-+.]\w+)*@\w+([-.]\w+)*\.\w+([-.]\w+)*$/',
       'url'       =>  '/^http(s?):\/\/(?:[A-za-z0-9-]+\.)+[A-za-z]{2,4}(:\d+)?(?:[\/\?#][\/=\?%\-&~`@[\]\':+!\.#\w]*)?$/',
       'currency'  =>  '/^\d+(\.\d+)?$/',
       'number'    =>  '/^\d+$/',
       'zip'       =>  '/^\d{6}$/',
       'integer'   =>  '/^[-\+]?\d+$/',
       'double'    =>  '/^[-\+]?\d+(\.\d+)?$/',
       'english'   =>  '/^[A-Za-z]+$/',
   );
   // 检查是否有内置的正则表达式
   if(isset($validate[strtolower($rule)]))
       $rule       =   $validate[strtolower($rule)];
   return preg_match($rule,$value)===1;
}


/**
 * 验证数据 支持 in between equal length regex expire ip_allow ip_deny
 * @access public
 * @param string $value 验证数据
 * @param mixed $rule 验证表达式
 * @param string $type 验证方式 默认为正则验证
 * @return boolean
 */
function common_check($value,$rule,$type='regex'){
   $type   =   strtolower(trim($type));
   switch($type) {
       case 'in': // 验证是否在某个指定范围之内 逗号分隔字符串或者数组
       case 'notin':
           $range   = is_array($rule)? $rule : explode(',',$rule);
           return $type == 'in' ? in_array($value ,$range) : !in_array($value ,$range);
       case 'between': // 验证是否在某个范围
       case 'notbetween': // 验证是否不在某个范围            
           if (is_array($rule)){
               $min    =    $rule[0];
               $max    =    $rule[1];
           }else{
               list($min,$max)   =  explode(',',$rule);
           }
           return $type == 'between' ? $value>=$min && $value<=$max : $value<$min || $value>$max;
       case 'equal': // 验证是否等于某个值
       case 'notequal': // 验证是否等于某个值            
           return $type == 'equal' ? $value == $rule : $value != $rule;
       case 'length': // 验证长度
           $length  =  mb_strlen($value,'utf-8'); // 当前数据长度
           if(strpos($rule,',')) { // 长度区间
               list($min,$max)   =  explode(',',$rule);
               return $length >= $min && $length <= $max;
           }else{// 指定长度
               return $length == $rule;
           }
       case 'expire':
           list($start,$end)   =  explode(',',$rule);
           if(!is_numeric($start)) $start   =  strtotime($start);
           if(!is_numeric($end)) $end   =  strtotime($end);
           return time() >= $start && time() <= $end;
       case 'ip_allow': // IP 操作许可验证
           return in_array(get_client_ip(),explode(',',$rule));
       case 'ip_deny': // IP 操作禁止验证
           return !in_array(get_client_ip(),explode(',',$rule));
       case 'regex':
       default:    // 默认使用正则验证 可以使用验证类中定义的验证名称
           // 检查附加规则
           return common_regex($value,$rule);
   }
}
