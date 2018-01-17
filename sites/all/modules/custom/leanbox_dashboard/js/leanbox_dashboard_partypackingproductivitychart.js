/**
 * @file
 * The js system, which integrates with google chart library.
 *
 */

(function ($) {

	Drupal.behaviors.leanbox_dashboard_party_packing_productivity = {
		attach: function (context, settings) {
			var area_definition = Drupal.settings.leanbox_dashboard.area_definition;
			var party_pack_productivity_data = Drupal.settings.leanbox_dashboard.party_packing_productivity;
			//google.charts.load("current", {packages: ['gauge','corechart', 'bar']});
			google.charts.setOnLoadCallback(function () {
				drawpartypackingproductivityChart(party_pack_productivity_data);
			});

			$(".party-packing-productivity-submit").click(function (e) {
				e.preventDefault();
			  var start_date = $(this).parent().prev().prev().find("input[name='start_date[date]']").val();
				var end_date = $(this).parent().prev().find("input[name='end_date[date]']").val();
				if (start_date != '' && end_date != '') {
					$.ajax({
						type: "POST",
						cache: false, //for Chrome and IE8
						url: "/chart-daterange-filter",
						data: {start_date: start_date, end_date: end_date, activity_type: 'party_packing_productivity'},
						success: function (response) {
							var area_definition = Drupal.settings.leanbox_dashboard.area_definition;
							google.charts.setOnLoadCallback(function () {
								drawpartypackingproductivityChart(response.output);
							});
							
							$('#otiftable').html(response.table);
						},
					});
				}
				
			});

			function drawpartypackingproductivityChart(party_pack_data) {

	      var data = google.visualization.arrayToDataTable(party_pack_data);
		
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
						title: 'Bill Lines/ManHr'
					},
					hAxis: {
						title: 'Section Wise'
					},
				};
				var chart = new google.visualization.ColumnChart(document.getElementById("party_packing_productivity_div"));
				//chart.draw(view, options);
				chart.draw(data, google.charts.Bar.convertOptions(options));
			}
		}
	};
})(jQuery);
