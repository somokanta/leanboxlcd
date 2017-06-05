/**
 * @file
 * The js system, which integrates with google chart library.
 *
 */

(function ($) {

	Drupal.behaviors.leanbox_dashboard_sales_delivery_table = {
		attach: function (context, settings) {
			
      $('#sales_delivery_table_div').css('height', '291px');
			var response = Drupal.settings.leanbox_dashboard.sales_delivery_table;
			$('#sales_delivery_table_div').html(response);
			$(".sales-delivery-table-submit").click(function (e) {

				e.preventDefault();
			  var start_date = $(this).parent().prev().prev().find("input[name='start_date[date]']").val();
				var end_date = $(this).parent().prev().find("input[name='end_date[date]']").val();
				if (start_date != '' && end_date != '') {
					$.ajax({
						type: "POST",
						cache: false, //for Chrome and IE8
						url: "/chart-daterange-filter",
						data: {start_date: start_date, end_date: end_date, activity_type: 'sales_delivery_table'},
						success: function (response) {
							$('#sales_delivery_table_div').html(response);
						},
					});
				}
				
			});
		}
	};
})(jQuery);
