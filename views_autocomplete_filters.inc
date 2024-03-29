<?php
/**
 * @file
 * Common functions for Views Autocomplete Filters module.
 */

/**
 * Menu callback. Retrieves a pipe delimited string of autocomplete suggestions.
 */
function views_autocomplete_filter($filter_name, $view_name, $display_name, $view_args, $string = '') {
  if (trim($string) == '') {
    // Ignore empty strings.
    return backdrop_json_output(array());
  }

  // If the search text has a '/', then the system will split it into
  // multiple arguments.
  $args = func_get_args();
  // Shift off the $filter_name, $view_name, $display_name, $view_args
  // arguments.
  for ($i = 0; $i < 4; $i++) {
    array_shift($args);
  }
  // Rebuild the search string.
  $string = implode('/', $args);

  // Get the view and set current display. The view and display were checked by
  // _views_autocomplete_filters_access_callback().
  $view = views_get_view($view_name);
  $view->set_display($display_name);
  if (!empty($view_args)) {
    $view->set_arguments(explode('||', $view_args));
  }
  // Set display and display handler vars for quick access.
  $display = $view->display[$display_name];
  $display_handler = $display->handler;

  // Get exposed filter options for our field.
  // Check if filter is exposed and autocomplete is enabled for this filter.
  // If filter exists/exposed at all.
  $filters = $display_handler->get_option('filters');
  if (empty($filters[$filter_name]['exposed']) || empty($filters[$filter_name]['expose']['autocomplete_filter'])) {
    return backdrop_not_found();
  }
  $filter = $filters[$filter_name];
  $expose_options = $filter['expose'];

  // Force "Display all values" for arguments set,
  // to get results no matter of Not Contextual filter present settings.
  if (!empty($expose_options['autocomplete_contextual']) && !$expose_options['autocomplete_contextual']) {
    $arguments = $display_handler->get_option('arguments');
    if (!empty($arguments)) {
      foreach ($arguments as $k => $argument) {
        $arguments[$k]['default_action'] = 'ignore';
      }
      $display_handler->set_option('arguments', $arguments);
    }
  }

  // Do not filter if the string length is less that minimum characters setting.
  if (isset($expose_options['autocomplete_min_chars']) && backdrop_strlen(trim($string)) < $expose_options['autocomplete_min_chars']) {
    $matches[''] = '<div class="reference-autocomplete reference-autocomplete--min-char">' . t('Search text should have at least %min_chars characters.', array('%min_chars' => $expose_options['autocomplete_min_chars'])) . '</div>';
    return backdrop_json_output($matches);
  }

  // Determine fields which will be used for output.
  if (empty($expose_options['autocomplete_field']) && !empty($filter['name'])) {
    if ($view->get_item($display_name, 'field', $filters[$filter_name]['id'])) {
      $field_name = $filters[$filter_name]['id'];
      // Force raw data for no autocomplete field defined.
      $expose_options['autocomplete_raw_suggestion'] = 1;
      $expose_options['autocomplete_raw_dropdown'] = 1;
    }
    else {
      // Field is not set, report about it to watchdog and return empty array.
      watchdog('views_autocomplete_filters', 'Field for autocomplete filter %label is not set in view %view, display %display', array(
        '%label' => $expose_options['label'],
        '%view' => $view->name,
        '%display' => $display->id,
      ), WATCHDOG_ERROR);
      return backdrop_json_output(array());
    }
  }
  // Text field autocomplete filter.
  elseif (!empty($expose_options['autocomplete_field'])) {
    $field_names = array($expose_options['autocomplete_field']);
  }
  // For combine fields autocomplete filter.
  elseif (!empty($filter['fields'])) {
    $field_names = array_keys($filter['fields']);
  }

  // Get fields options and check field exists in this display.
  foreach ($field_names as $field_name) {
    $field_options = $view->get_item($display_name, 'field', $field_name);
    if (empty($field_options)) {
      // Field not exists, report about it to watchdog and return empty array.
      watchdog('views_autocomplete_filters', 'Field for autocomplete filter %label not exists in view %view, display %display', array(
        '%label' => $expose_options['label'],
        '%view' => $view->name,
        '%display' => $display->id,
      ), WATCHDOG_ERROR);
      return backdrop_json_output(array());
    }
  }

  // Collect exposed filter values and set them to the view.
  if (!empty($expose_options['autocomplete_dependent'])) {
    $exposed_input = $_GET;
  }
  else {
    $exposed_input = array();
  }
  $exposed_input[$expose_options['identifier']] = $string;
  $view->set_exposed_input($exposed_input);

  // Disable cache for view, because caching autocomplete is a expensive.
  $display_handler->set_option('cache', array('type' => 'none'));

  // Force limit for results.
  if (empty($expose_options['autocomplete_items'])) {
    $pager_type = 'none';
  }
  else {
    $pager_type = 'some';
  }
  $pager = array(
    'type' => $pager_type,
    'options' => array(
      'items_per_page' => $expose_options['autocomplete_items'],
      'offset' => 0,
    ),
  );
  $display_handler->set_option('pager', $pager);

  // Execute view query.
  $view->pre_execute();
  $queryOptions = $display_handler->get_option('query');
  $queryOptions['options']['distinct'] = '1';
  $queryOptions['options']['pure_distinct'] = '1';
  $display_handler->set_option('query', $queryOptions);
  foreach ($view->field as $field_name => $field_obj) {
    if (!in_array($field_name, $field_names)) {
      unset($view->field[$field_name]);
      unset($view->display_handler->handlers['field'][$field_name]);
    }
  }
  $view->execute($view->current_display);
  $view->post_execute();

  // Render it seems required by Search API Views integration.
  if ($view->base_field == 'search_api_id' && stristr($view->base_table, 'search_api_index_')) {
    $view->render();
  }

  // Render field on each row and fill matches array.
  $use_raw_suggestion = !empty($expose_options['autocomplete_raw_suggestion']);
  $use_raw_dropdown = !empty($expose_options['autocomplete_raw_dropdown']);

  $matches = array();
  $view->row_index = 0;
  foreach ($view->result as $index => $row) {
    $view->row_index = $index;
    $rendered_field = $raw_field = '';

    foreach ($field_names as $field_name) {
      // Render field only if suggestion or dropdown item not in RAW format.
      if (!$use_raw_suggestion || !$use_raw_dropdown) {
        $rendered_field = $view->style_plugin->get_field($index, $field_name);
      }

      // Get the raw field value only if suggestion or dropdown item is in RAW
      // format.
      if ($use_raw_suggestion || $use_raw_dropdown) {
        $raw_field = $view->style_plugin->get_field_value($index, $field_name);
        if (!is_array($raw_field)) {
          $raw_field = array(array('value' => $raw_field));
        }
      }

      if (empty($raw_field)) {
        $raw_field = array(array('value' => $rendered_field));
      }

      foreach ($raw_field as $delta => $item) {
        $dropdown = '';
        // Use transliteration if available.
        if (function_exists('transliteration_get')) {
          if (isset($item['value']) && strstr(transliteration_get(decode_entities(backdrop_strtolower($item['value']))), transliteration_get(backdrop_strtolower($string)))) {
            $dropdown = $use_raw_dropdown ? check_plain($item['value']) : $rendered_field;
          }
        }
        elseif (isset($item['value']) && strstr(backdrop_strtolower($item['value']), backdrop_strtolower($string))) {
          $dropdown = $use_raw_dropdown ? check_plain($item['value']) : $rendered_field;
        }
        if ($dropdown != '') {
          $suggestion = $use_raw_suggestion ? check_plain($item['value']) : $rendered_field;
          $suggestion = decode_entities($suggestion);

          // Add a class wrapper for a few required CSS overrides.
          $matches[$suggestion] = '<div class="reference-autocomplete">' . $dropdown . '</div>';
        }
      }
    }
  }
  unset($view->row_index);

  if (empty($matches)) {
    $matches[''] = '<div class="reference-autocomplete reference-autocomplete--empty-result">' . t('%string returned no results. Please try something else.', array('%string' => $string)) . '</div>';
  }

  // Allow modules to alter the calculated matches or programmatically change
  // empty results messages.
  backdrop_alter('views_autocomplete_filter_matches', $matches, $string, $view);

  return backdrop_json_output($matches);
}
