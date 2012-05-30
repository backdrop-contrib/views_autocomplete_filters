<?php
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
    drupal_json_output($autocomplete_filter_results[$results_id]);
    return;
  }
  $matches = array();
  $view = views_get_view($view_name);
  if(!$view) {
    $matches[''] = '<div class="reference-autocomplete">'. t('No matches were found for %string.', array('%string' => $string)) . '</div>';
    drupal_json_output($autocomplete_filter_results[$results_id]);
    return;
  }
  $view->set_display($view_display);
  // force to have no limit for results
  $view->display_handler->set_option('items_per_page', 0);
  $view->handler->items_per_page = $view->pager['items_per_page'] = 0;
  $currentview = $view->display[$view_display];
  // exposed form field name
  if ($currentview->handler->options['defaults']['filters']) {
    $currentview = $view->display['default'];
  }
  else {
   $currentview = $view->display[$view_display];
  }
  $field_filter = $currentview->handler->options['filters'][$field_name];
  $real_field_name = str_replace('_value', '', $field_name);
  // assign string value to the exposed text filter
  $view->exposed_input[$field_filter['expose']['identifier']] = $string;

  $item = $view->get_item($view_display, 'filter', $field_name);
  // overwrite the fields for the view
  $view->display[$view_display]->handler->options['fields'] = array($real_field_name => $view->display[$view_display]->handler->options['fields'][$real_field_name]);
  $view->display[$view_display]->display_options['fields'] = array($real_field_name => $view->display[$view_display]->handler->options['fields'][$real_field_name]);
  $view->execute($view->current_display);
 
  $currentview = $view->display[$view_display];
  // views field handler data
  $field = $currentview->handler->handlers['field'][$real_field_name];
  $field_alias = $field->field_alias;
  // check if it's a field
  if(0 === strpos($field->table,'field_data')){
    $field_alias_name = 'field_'.$field->field;
    foreach ($view->result as $id => $row) {
    // Add a class wrapper for a few required CSS overrides.
      if (!empty($row->$field_alias)) {
        $x = $row->$field_alias_name;
        $matches[$x[0]['rendered']['#markup']] = '<div class="reference-autocomplete">'. $x[0]['raw']['safe_value'] .'</div>';
      }
    }
  }
  else {
    foreach ($view->result as $id => $row) {
      // Add a class wrapper for a few required CSS overrides.
      if (!empty($row->$field_alias)) {
        $matches[$row->$field_alias] = '<div class="reference-autocomplete">'. $row->$field_alias .'</div>';
      }
    }
  }
  if (!empty($matches)) {
    $autocomplete_filter_results[$results_id] = $matches;
  }
  elseif (!empty($view->result)) {
    $matches[''] = '<div class="reference-autocomplete">'. t('The %field is required but is missing or excluded from display.', array('%field' => $field_filter['expose']['label'])) . '</div>';
  }
  else {
    $matches[''] = '<div class="reference-autocomplete">'. t('No matches were found for %string.', array('%string' => $string)) . '</div>';
  }
  drupal_json_output($matches);
}