/**
 * @file
 * The js system, which integrates with google chart library.
 *
 */

(function ($) {

	Drupal.behaviors.leanbox_mobile_fullreturnchart = {
		attach: function (context, settings) {

			var party_pack_data = Drupal.settings.leanbox_mobile.fullreturnchart;
			google.charts.load("current", {packages: ['gauge', 'corechart', 'bar']});
			google.charts.setOnLoadCallback(function () {
				drawpartypackingChart(party_pack_data);
			});

			$(".mobile-full-return-submit").click(function (e) {

				e.preventDefault();
				var start_date = $(this).parent().prev().prev().find("input[name='start_date[date]']").val();
				var end_date = $(this).parent().prev().find("input[name='end_date[date]']").val();
				if (start_date != '' && end_date != '') {
					$.ajax({
						type: "POST",
						cache: false, //for Chrome and IE8
						url: "/leanbox-mobile-filter",
						data: {start_date: start_date, end_date: end_date, activity_type: 'mobile_full_return_graph'},
						success: function (response) {

							google.charts.setOnLoadCallback(function () {
								drawpartypackingChart(response);
							});
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
				var chart = new google.visualization.PieChart(document.getElementById("mobile_full_return_div"));
				chart.draw(view, options);
				//chart.draw(view, google.charts.Bar.convertOptions(options));
			}
		}
	};
})(jQuery);
