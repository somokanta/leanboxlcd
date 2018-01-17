/**
 * @file
 * The js system, which integrates with google chart library.
 *
 */

(function ($) {

	Drupal.behaviors.all_chart_load = {
		attach: function (context) {
                   google.charts.load("current", {packages: ['gauge','corechart', 'bar']});
		}
	};
})(jQuery);