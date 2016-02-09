<?php

/**
 * @file
 * Hooks provided by Views Autocomplete Filters.
 */


/**
 * Allow modules to alter the calculated matches or programmatically change
 * empty results messages.
 *
 * @param array $matches
 *   Markup for autocomplete matches, keyed by suggestion text.
 * @param string $string
 *   The base string entered by the user within the from.
 * @param stdClass $view
 *   The executed view object used to lookup match suggestions.
 */
function hook_views_autocomplete_filter_matches_alter(&$matches, $string, $view) {
  // Disable any empty result messages.
  if (isset($matches['']) && count($matches) == 1) {
    $matches = array();
  }
}
