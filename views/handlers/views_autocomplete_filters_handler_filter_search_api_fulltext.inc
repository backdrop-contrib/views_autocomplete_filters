<?php

/**
 * @file
 * Definition of views_autocomplete_filters_handler_filter_string.
 */

/**
 * Extend the basic textfield filter handler with autocomplete.
 *
 * @ingroup views_filter_handlers
 */
class views_autocomplete_filters_handler_filter_search_api_fulltext extends SearchApiViewsHandlerFilterFulltext {
  // Exposed filter options.
  var $always_multiple = TRUE;

  function option_definition() {
    $options = parent::option_definition();

    // Use helper function, avoid code duplicates.
    views_autocomplete_filters_filter_hander_option_definition($options);

    return $options;
  }

  function expose_form(&$form, &$form_state) {
    parent::expose_form($form, $form_state);

    // Use helper function, avoid code duplicates.
    views_autocomplete_filters_filter_hander_expose_form($form, $form_state, $this);
  }

  public function value_form(&$form, &$form_state) {
    parent::value_form($form, $form_state);

    // Use helper function, avoid code duplicates.
    views_autocomplete_filters_filter_hander_value_form($form, $form_state, $this);
  }

  /**
   * Validate that this filter instance has a corresponding autocomplete results field.
   *
   * @return array
   *   An array of errors triggered by this validation.
   */
  public function validate() {
    $errors = parent::validate();

    // Use helper function, avoid code duplicates.
    views_autocomplete_filters_filter_hander_validate($errors, $this);

    return $errors;
  }
}
