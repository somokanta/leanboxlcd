/**
 * @file
 * The js system, which integrates with google chart library.
 *
 */

(function ($) {

	Drupal.behaviors.leanbox_dashboard_bills_per_van_trends = {
		attach: function (context, settings) {
			
      $('#bills_per_van_trends_div').css('height', '291px');
			var response = Drupal.settings.leanbox_dashboard.bills_per_van_trends;
			$('#bills_per_van_trends_div').html(response);
			$(".bills-per-van-trends-submit").click(function (e) {

				e.preventDefault();
			  var start_date = $(this).parent().prev().prev().find("input[name='start_date[date]']").val();
				var end_date = $(this).parent().prev().find("input[name='end_date[date]']").val();
				if (start_date != '' && end_date != '') {
					$.ajax({
						type: "POST",
						cache: false, //for Chrome and IE8
						url: "/chart-daterange-filter",
						data: {start_date: start_date, end_date: end_date, activity_type: 'bills_per_van_trends'},
						success: function (response) {
							$('#bills_per_van_trends_div').html(response);
						},
					});
				}
				
			});
		}
	};
})(jQuery);
