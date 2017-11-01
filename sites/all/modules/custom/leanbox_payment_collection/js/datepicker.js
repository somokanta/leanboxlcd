/**
 * @file
 * The js system, which integrates with google chart library.
 *
 */

(function ($) {

	Drupal.behaviors.leanbox_datepicker = {
		attach: function (context) {
			// write your code here
                        $('.cheque-date-field').datepicker({                        
                dateFormat: "yy-mm-dd"
            });
		}
	};
})(jQuery);