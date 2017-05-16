/**
 * @file
 * The js system, which integrates with google chart library.
 *
 */

(function ($) {

	Drupal.behaviors.leanbox_dashboard_otif_trends = {
		attach: function (context, settings) {
			
			var otif = Drupal.settings.leanbox_dashboard.otif_trends;
      google.charts.load('current', {'packages':['gauge','corechart', 'bar']});
			google.charts.setOnLoadCallback(function () {
				drawotifChart(otif);
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
							google.charts.setOnLoadCallback(function () {
								drawotifChart(response.output);
							});
							$('#otiftable').html(response.table);
						},
					});
				}
				
			});

			function drawotifChart(otif) {

        var data = google.visualization.arrayToDataTable([
          ['Label', 'Value'],
          ['OTIF', otif],
        ]);

        var options = {
					curveType: 'function',
          title: 'OTIF Trends',
        };

       // var chart = new google.visualization.Gauge(document.getElementById('otif_div'));
				var chart = new google.visualization.LineChart(document.getElementById('otif_trends_div'));

        chart.draw(data, options);
				
			}
		}
	};
})(jQuery);
