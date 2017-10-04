/**
 * @file
 * The js system, which integrates with google chart library.
 *
 */

(function ($) {

	Drupal.behaviors.leanbox_dashboard_central_dispatch_daywise = {
		attach: function (context, settings) {

			$('#central_dispatch_daywise_div').css('height', '291px');
			var response = Drupal.settings.leanbox_dashboard.central_dispatch_daywise;
			$('#central_dispatch_daywise_div').html(response);
			$(".central-dispatch-daywise-submit").click(function (e) {

				e.preventDefault();
			  var start_date = $(this).parent().prev().prev().find("input[name='start_date[date]']").val();
				var end_date = $(this).parent().prev().find("input[name='end_date[date]']").val();
				if (start_date != '' && end_date != '') {
					$.ajax({
						type: "POST",
						cache: false, //for Chrome and IE8
						url: "/chart-daterange-filter",
						data: {start_date: start_date, end_date: end_date, activity_type: 'central_dispatch_daywise'},
						success: function (response) {
							$('#central_dispatch_daywise_div').html(response);
						},
					});
				}
				
			});
		}
	};
})(jQuery);
