/**
 * @file
 * The js system, which integrates with google chart library.
 *
 */

(function ($) {

	Drupal.behaviors.leanbox_dashboard_unloading = {
		attach: function (context, settings) {

			var area_definition = Drupal.settings.leanbox_dashboard.area_definition;
			var unloading_data = Drupal.settings.leanbox_dashboard.unloading;
			google.charts.load("current", {packages: ['gauge', 'corechart', 'bar']});
			google.charts.setOnLoadCallback(function () {
				drawunloadingChart(unloading_data);
			});

			$(".unloading-submit").click(function (e) {

				e.preventDefault();
				var start_date = $(this).parent().prev().prev().find("input[name='start_date[date]']").val();
				var end_date = $(this).parent().prev().find("input[name='end_date[date]']").val();
				if (start_date != '' && end_date != '') {
					$.ajax({
						type: "POST",
						cache: false, //for Chrome and IE8
						url: "/chart-daterange-filter",
						data: {start_date: start_date, end_date: end_date, activity_type: 'unloading'},
						success: function (response) {

							var area_definition = Drupal.settings.leanbox_dashboard.area_definition;
							google.charts.setOnLoadCallback(function () {
								drawunloadingChart(response);
							});
						},
					});
				}
			});

			function drawunloadingChart(unloading_data) {
				var data = google.visualization.arrayToDataTable(unloading_data);

				var view = new google.visualization.DataView(data);
				var formatPercent = new google.visualization.NumberFormat({
					pattern: '#,##0.0%'
				});
				view.setColumns([   0,
					1, {
						calc: function (dt, row) {
							return dt.getValue(row, 1) + '(' +  parseFloat(formatPercent.formatValue(dt.getValue(row, 1) / (dt.getValue(row, 1) + dt.getValue(row, 2) + dt.getValue(row, 3)))).toFixed(0) + '%)';
						},
						sourceColumn: 1,
						type: "string",
						role: "annotation", },
					2, {
						calc: function (dt, row) {
							return dt.getValue(row, 2) + '(' +  parseFloat(formatPercent.formatValue(dt.getValue(row, 2) / (dt.getValue(row, 1) + dt.getValue(row, 2) + dt.getValue(row, 3)))).toFixed(0) + '%)';
						},
						sourceColumn: 2,
						type: "string",
						role: "annotation"},
					3, {
						calc: function (dt, row) {
							return dt.getValue(row, 3) + '(' +  parseFloat(formatPercent.formatValue(dt.getValue(row, 3) / (dt.getValue(row, 1) + dt.getValue(row, 2) + dt.getValue(row, 3)))).toFixed(0) + '%)';
						},
						sourceColumn: 3,
						type: "string",
						role: "annotation"},
				]);
				var options = {
					width: area_definition.width,
					height: area_definition.height,
					title: '',
					legend: {position: 'top', maxLines: 3},
					bar: {groupWidth: '60%'},
					chartArea: {left: area_definition.ch_left, top: area_definition.ch_top, width: area_definition.ch_width, height: area_definition.ch_height},
					series: {
						0: {color: '#e74c3c'},
						1: {color: '#f39c12'},
						2: {color: '#27ae60'}
					},
				//	isStacked: 'percent',
					vAxis: {
						minValue: 0,
						maxValue: 1,
						title: 'Unloading Activity',
						 // format: "percent",
					},
				};
				var chart = new google.visualization.ColumnChart(document.getElementById("unloading_div"));
				//chart.draw(view, options);
				chart.draw(view, google.charts.Bar.convertOptions(options));
			}
		}
	};
})(jQuery);
