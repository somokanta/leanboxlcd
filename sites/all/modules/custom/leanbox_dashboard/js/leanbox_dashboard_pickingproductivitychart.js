/**
 * @file
 * The js system, which integrates with google chart library.
 *
 */

(function ($) {

	Drupal.behaviors.leanbox_dashboard_picking_productivity = {
		attach: function (context, settings) {
			var area_definition = Drupal.settings.leanbox_dashboard.area_definition;
			var picking_productivity_data = Drupal.settings.leanbox_dashboard.picking_productivity;
			google.charts.load("current", {packages: ['gauge','corechart', 'bar']});
			google.charts.setOnLoadCallback(function () {
				drawpickingproductivityChart(picking_productivity_data);
			});

			$(".picking-productivity-submit").click(function (e) {

				e.preventDefault();
			  var start_date = $(this).parent().prev().prev().find("input[name='start_date[date]']").val();
				var end_date = $(this).parent().prev().find("input[name='end_date[date]']").val();
				if (start_date != '' && end_date != '') {
					$.ajax({
						type: "POST",
						cache: false, //for Chrome and IE8
						url: "/chart-daterange-filter",
						data: {start_date: start_date, end_date: end_date, activity_type: 'picking_productivity'},
						success: function (response) {
							var area_definition = Drupal.settings.leanbox_dashboard.area_definition;
							google.charts.setOnLoadCallback(function () {
								drawpickingproductivityChart(response.output);
							});
							$('#otiftable').html(response.table);
						},
					});
				}
				
			});

			function drawpickingproductivityChart(picking_data) {

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
				]);
				var options = {
					width: area_definition.width,
					height: area_definition.height,
					title: '',
					legend: {position: 'top', maxLines: 3},
					bar: {groupWidth: '40%'},
					chartArea: {left: area_definition.ch_left, top: area_definition.ch_top, width: area_definition.ch_width, height: area_definition.ch_height},
					series: {
			
					},
					vAxis: {
						minValue: 0,
						maxValue: 7,
						title: 'Picking Productivity'
					},
					hAxis: {
						title: 'Picking Productivity'
					},
				};
				var chart = new google.visualization.ColumnChart(document.getElementById("picking_productivity_div"));
			//	chart.draw(view, options);
			chart.draw(data, google.charts.Bar.convertOptions(options));
			}
		}
	};
})(jQuery);
