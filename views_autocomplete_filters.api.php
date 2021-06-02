<?php
/**
 * @file
 * Hooks provided by Views Autocomplete Filters.
 */

/**
 * Alter the calculated matches or empty results messages.
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
