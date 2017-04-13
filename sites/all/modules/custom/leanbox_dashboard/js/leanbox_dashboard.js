/**
 * @file
 * The js system, which integrates with google chart library.
 *
 */

(function ($) {

	Drupal.behaviors.leanbox_dashboard = {
		attach: function (context) {
			setTimeout(function () {
				window.location.reload(1);
			}, 30000); // every 5 minute
		}
	};
})(jQuery);