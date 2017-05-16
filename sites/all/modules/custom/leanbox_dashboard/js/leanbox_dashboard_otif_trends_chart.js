/**
 * @file
 * The js system, which integrates with google chart library.
 *
 */

(function ($) {

	Drupal.behaviors.leanbox_dashboard_otif_trends = {
		attach: function (context, settings) {
			
			var otif = Drupal.settings.leanbox_dashboard.otif_trends;
			var area_definition = Drupal.settings.leanbox_dashboard.area_definition;
			console.log(otif);
      google.charts.load('current', {'packages':['gauge','corechart', 'bar']});
			google.charts.setOnLoadCallback(function () {
				drawotifChart(otif, area_definition);
			});

			$(".otif-trends-submit").click(function (e) {

				e.preventDefault();
			  var start_date = $(this).parent().prev().prev().find("input[name='start_date[date]']").val();
				var end_date = $(this).parent().prev().find("input[name='end_date[date]']").val();
				if (start_date != '' && end_date != '') {
					$.ajax({
						type: "POST",
						cache: false, //for Chrome and IE8
						url: "/chart-daterange-filter",
						data: {start_date: start_date, end_date: end_date, activity_type: 'otif_trends'},
						success: function (response) {
							var area_definition = Drupal.settings.leanbox_dashboard.area_definition;
							
							google.charts.setOnLoadCallback(function () {
								drawotifChart(response.output, area_definition);
							});
							$('#otiftable').html(response.table);
						},
					});
				}
				
			});

			function drawotifChart(otif, area_definition) {

        var data = google.visualization.arrayToDataTable(otif);

        var options = {
					width: area_definition.width,
					height: area_definition.height,
					chartArea: {left: area_definition.ch_left, top: area_definition.ch_top, width: area_definition.ch_width, height: area_definition.ch_height},
					curveType: 'function',
          title: '',
					legend: {position: 'top', maxLines: 3},
        };

       // var chart = new google.visualization.Gauge(document.getElementById('otif_div'));
				var chart = new google.visualization.LineChart(document.getElementById('otif_trends_div'));

        chart.draw(data, options);
				
			}
		}
	};
})(jQuery);
