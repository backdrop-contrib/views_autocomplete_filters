Views Autocomplete Filters
======================

Autocomplete functionality for node titles and text fields.

The autocomplete results will be the results from the view.


Requirements
------------

This module requires that the following modules are also enabled:

 * Views


Installation
------------

- Install this module using the official Backdrop CMS instructions at
  https://backdropcms.org/guide/modules.

- Configure a View from the Views page under Administration > Structure > Views
  (admin/structure/views).

- Configure an exposed text field filter and check `Use Autocomplete checkbox`
  in the exposed filter settings.


Tips & Tricks
-------------

- To work as you wish you have to make the right choice for the operator of
  filter field, most likely to use "Contains"

- For referenced entities (entity reference, node references, term reference)
  use referenced entity fields for the filters with the right relationships.

- If you have more titles fields defined in fields and/or filters the
  correspondence between field & filter must be right. For example: if field
  is title_2 then also the filter field should be title_2.


Documentation
-------------

Additional documentation is located in the Wiki:
https://github.com/backdrop-contrib/views_autocomplete_filters/wiki/Documentation.

Video Tutorial: [Drupal 7 Views Autocomplete Filter Tutorial](http://codekarate.com/daily-dose-of-drupal/drupal-7-views-autocomplete-filter)


Issues
------

Bugs and Feature requests should be reported in the Issue Queue:
https://github.com/backdrop-contrib/views_autocomplete_filters/issues.


Current Maintainers
-------------------

- [Justin Christoffersen](https://github.com/jenlampton).
- [Jen Lampton](https://github.com/jenlampton).
- Seeking additional maintainers.


Credits
-------

- Ported to Backdrop CMS by [Justin Christoffersen](https://github.com/jenlampton).
- Ported to Backdrop CMS by [Jen Lampton](https://github.com/jenlampton).
- Backdrop port sponsored by [Ivar Jacoboen International](https://www.ivarjacobson.com)
- Originally written for Drupal by [Tavi Toporjinschi](https://github.com/vasike).
- Drupal maintenance sponsored by [TOP EXPRESS](http://www.topexpress.ro/)
- Initial Drupal version sponsored by [viennaresidence](http://www.viennaresidence.com/)


License
-------

This project is GPL v2 software.
See the LICENSE.txt file in this directory for complete text.

