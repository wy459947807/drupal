<?php

/**
 * @file
 * Default theme implementation to wrap aggregator content.
 *
 * Available variables:
 * - $content: All aggregator content.
 * - $page: Pager links rendered through theme_pager().
 *
 * @see template_preprocess()
 * @see template_preprocess_aggregator_wrapper()
 *
 * @ingroup themeable
 */
?>
<div id="aggregator">
    <?php echo $dataInfo['test']; ?>
    <?php echo MODULE_PATH?>
    <?php echo url("http://www.baidu.com/",array("query"=>array("theme"=>"bartik")));?>
</div>
<link rel="stylesheet" type="text/css" media="all" href="<?php echo MODULE_PATH?>/css/annotate.css">
