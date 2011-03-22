<?php
// $Id: views_autocomplete_filters.module,v 1 vasike Exp $

/**
 * @file
 * Common functions for Autocomplete Filters module.
 */

/**
 * Menu callback; Retrieve a pipe delimited string of autocomplete suggestions.
 */
function views_autocomplete_filter($field_name = NULL, $view_name, $view_display, $string = '') {
 static $autocomplete_filter_results;
 $results_id = $field_name.'_'.$view_name.'_'.$view_display.'_'.$string;
 if (isset($autocomplete_filter_results[$results_id])) {
	  drupal_json($autocomplete_filter_results[$results_id]);
		return;
 }
 $matches = array();
 //$view_args = array();
 $view = views_get_view($view_name);
 $view->set_display($view_display);
 $view->display_handler->set_option('items_per_page', 0);
 $currentview = $view->display[$view_display];
 // exposed form field name
 $field_filter = $currentview->handler->options['filters'][$field_name];
 // assign string value to the exposed text filter
 $view->exposed_input[$field_filter['expose']['identifier']] = $string;
 $view->execute($view->current_display);
 $currentview = $view->display[$view_display];
 // views field handler data
 $field = $currentview->handler->handlers['field'][$field_name];
 $field_alias = $field->field_alias;
 //print_r($view);

 foreach ($view->result as $id => $row) {
   // Add a class wrapper for a few required CSS overrides.
	 if (!empty($row->$field_alias)) {
	    $matches[$row->$field_alias] = '<div class="reference-autocomplete">'. $row->$field_alias .'</div>';
	 }
 }
 if (!empty($matches)) {
	 $autocomplete_filter_results[$results_id] = $matches;
 } else {
	 $matches[''] = '<div class="reference-autocomplete">'. t('The %field field of this filter should be present also in the result (could be excluded from display).', array('%field' => $field_filter['expose']['label'])) . '</div>';
 }
 drupal_json($matches);
}