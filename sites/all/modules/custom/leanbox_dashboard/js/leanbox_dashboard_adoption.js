/**
 * @file
 * The js system, which integrates with google chart library.
 *
 */

(function ($) {

	Drupal.behaviors.leanbox_dashboard_adoption_graph = {
		attach: function (context, settings) {

			var delivery_data = Drupal.settings.leanbox_dashboard.adoption_graph;
			var area_definition = Drupal.settings.leanbox_dashboard.area_definition;
			google.charts.load("current", {packages: ['gauge','corechart', 'bar']});
			google.charts.setOnLoadCallback(function () {
				drawpickingChart(delivery_data, area_definition);
			});

			$(".adoption-graph-submit").click(function (e) {

				e.preventDefault();
			  var start_date = $(this).parent().prev().prev().find("input[name='start_date[date]']").val();
				var end_date = $(this).parent().prev().find("input[name='end_date[date]']").val();
				if (start_date != '' && end_date != '') {
					$.ajax({
						type: "POST",
						cache: false, //for Chrome and IE8
						url: "/chart-daterange-filter",
						data: {start_date: start_date, end_date: end_date, activity_type: 'adoption_graph'},
						success: function (response) {
							var area_definition = Drupal.settings.leanbox_dashboard.area_definition;
							google.charts.setOnLoadCallback(function () {
								drawpickingChart(response, area_definition);
							});
						},
					});
				}
			});

			function drawpickingChart(delivery_data, area_definition) {
                            console.log(delivery_data);
                                                        console.log(area_definition);

				var data = google.visualization.arrayToDataTable(delivery_data);

				var options = {
					width: area_definition.width,
					height: area_definition.height,
					title: '',
					legend: {position: 'top', maxLines: 3},
            
					bar: {groupWidth: '40%'},
					//chartArea: {left: area_definition.ch_left, top: area_definition.ch_top, width: area_definition.ch_width, height: area_definition.ch_height},

					vAxis: {
						minValue: 0,
						maxValue: 7,
						title: 'Count'
					},
					hAxis: {
						title: ''
					},
				};
				var chart = new google.visualization.ColumnChart(document.getElementById('adoption_graph_div'));
				chart.draw(data, options);
			}
		}
	};
})(jQuery);
