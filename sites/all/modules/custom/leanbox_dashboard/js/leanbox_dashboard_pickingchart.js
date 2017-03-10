/**
 * @file
 * The js system, which integrates with google chart library.
 *
 */

(function ($) {

	Drupal.behaviors.leanbox_dashboard_picking = {
		attach: function (context, settings) {

			var picking_data = Drupal.settings.leanbox_dashboard.picking;
			var area_definition = Drupal.settings.leanbox_dashboard.area_definition;
			google.charts.load("current", {packages: ['corechart', 'bar']});
			google.charts.setOnLoadCallback(function () {
				drawpickingChart(picking_data, area_definition);
			});

			$(".picking-submit").click(function (e) {

				e.preventDefault();
			  var start_date = $(this).parent().prev().prev().find("input[name='start_date[date]']").val();
				var end_date = $(this).parent().prev().find("input[name='end_date[date]']").val();
				if (start_date != '' && end_date != '') {
					$.ajax({
						type: "POST",
						cache: false, //for Chrome and IE8
						url: "/chart-daterange-filter",
						data: {start_date: start_date, end_date: end_date, activity_type: 'picking'},
						success: function (response) {
							
							var area_definition = Drupal.settings.leanbox_dashboard.area_definition;
							google.charts.load("current", {packages: ['corechart', 'bar']});
							google.charts.setOnLoadCallback(function () {
								drawpickingChart(response, area_definition);
							});
						},
					});
				}
			});

			function drawpickingChart(picking_data, area_definition) {
				var data = google.visualization.arrayToDataTable(picking_data);

				var view = new google.visualization.DataView(data);
				view.setColumns([0,
					1, {calc: "stringify",
						sourceColumn: 1,
						type: "string",
						role: "annotation"},
					2, {calc: "stringify",
						sourceColumn: 2,
						type: "string",
						role: "annotation"},
					3, {calc: "stringify",
						sourceColumn: 3,
						type: "string",
						role: "annotation"},
				]);
				var options = {
					width: area_definition.width,
					height: area_definition.height,
					title: 'Picking Chart',
					legend: {position: 'right', maxLines: 3},
					bar: {groupWidth: '40%'},
					chartArea: {left: area_definition.ch_left, top: area_definition.ch_top, width: area_definition.ch_width, height: area_definition.ch_height},
					series: {
						0: {color: '#e74c3c'},
						1: {color: '#f39c12'},
						2: {color: '#27ae60'}
					},
					isStacked: true,
					vAxis: {
						minValue: 0,
						maxValue: 7,
						title: 'Picking Activity'
					},
					hAxis: {
						title: 'Pricelist Group'
					},
				};
				var chart = new google.visualization.ColumnChart(document.getElementById('picking_div'));
				chart.draw(view, options);
			}
		}
	};
})(jQuery);
