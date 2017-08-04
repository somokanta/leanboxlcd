/**
 * @file
 * The js system, which integrates with google chart library.
 *
 */

(function ($) {

	Drupal.behaviors.leanbox_dashboard_returnchart = {
		attach: function (context, settings) {

			var area_definition = Drupal.settings.leanbox_dashboard.area_definition;
			var party_pack_data = Drupal.settings.leanbox_dashboard.returnchart;
			google.charts.load("current", {packages: ['gauge', 'corechart', 'bar']});
			google.charts.setOnLoadCallback(function () {
				drawpartypackingChart(party_pack_data);
			});

			$(".bill-pendency-submit").click(function (e) {

				e.preventDefault();
				var start_date = $(this).parent().prev().prev().find("input[name='start_date[date]']").val();
				var end_date = $(this).parent().prev().find("input[name='end_date[date]']").val();
				if (start_date != '' && end_date != '') {
					$.ajax({
						type: "POST",
						cache: false, //for Chrome and IE8
						url: "/chart-daterange-filter",
						data: {start_date: start_date, end_date: end_date, activity_type: 'mobile_return_graph'},
						success: function (response) {

							var area_definition = Drupal.settings.leanbox_dashboard.area_definition;
							google.charts.setOnLoadCallback(function () {
								drawpartypackingChart(response.output);
							});
							$('#googlechart').html(response.table);
						},
					});
				}

			});

			function drawpartypackingChart(party_pack_data) {

				var data = google.visualization.arrayToDataTable(party_pack_data);

				var view = new google.visualization.DataView(data);
	
				var options = {

					title: '',
					//legend: {position: 'top', maxLines: 3},

				};
				var chart = new google.visualization.PieChart(document.getElementById("mobile_return_div"));
				chart.draw(view, options);
				//chart.draw(view, google.charts.Bar.convertOptions(options));
			}
		}
	};
})(jQuery);
