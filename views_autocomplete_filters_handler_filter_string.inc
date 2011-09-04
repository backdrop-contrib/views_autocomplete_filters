<?php

/**
 * Basic textfield filter to handle string filtering commands
 * including equality, like, not like, etc.
 */
class views_autocomplete_filters_handler_filter_string extends views_handler_filter_string {
  // exposed filter options
  var $always_multiple = TRUE;

  function option_definition() {
    $options = parent::option_definition();

    $options['expose']['contains']['autocomplete_filter'] = array('default' => 0);

    return $options;
  }

  function expose_form(&$form, &$form_state) {
    parent::expose_form($form, $form_state);
    $form['expose']['autocomplete_filter'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use Autocomplete'),
      '#default_value' => !empty($this->options['expose']['autocomplete_filter']),
      '#description' => t('Use Autocomplete for this filter.'),
    );
  }
}