<?php
/**
 * Implements hook_html_head_alter().
 * This will overwrite the default meta character type tag with HTML5 version.
 */
function annotate_html_head_alter(&$head_elements) {
  $head_elements['system_meta_content_type']['#attributes'] = array(
    'charset' => 'utf-8'
  );
}

/**
 * Override or insert variables into the html template.
 */
function annotate_process_html(&$vars) {
  // Hook into color.module
  if (module_exists('color')) {
    _color_html_alter($vars);
  }
}

/**
 * Override or insert variables into the page template.
 */
function annotate_process_page(&$variables) {
  global $user;
  $uri = $_SERVER['REQUEST_URI'];
  
  if(isset($variables['node']->type)&&$variables['node']->type=='article'){
   
  }

}
/**
 * Override or insert variables into the page template.
 */
function annotate_preprocess_page(&$vars) {
  
  
}

/**
 * Override or insert variables into the node template.
 */
function annotate_preprocess_node(&$variables) {
  $node = $variables['node'];
  if ($variables['view_mode'] == 'full' && node_is_page($variables['node'])) {
    $variables['classes_array'][] = 'node-full';
  }
}


