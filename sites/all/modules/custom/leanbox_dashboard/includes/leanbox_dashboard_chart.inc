<?php

/**
 * 
 * @param text $arg
 * @return html
 */
function leanbox_dashboard_chart_details_callback($arg) {
 $block = module_invoke('leanbox_dashboard', 'block_view', $arg);
 $output =  render($block['content']);
 return $output;
}