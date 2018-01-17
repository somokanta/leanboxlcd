/**
 * @file
 * The js system, which integrates with google chart library.
 *
 */

(function ($) {

	Drupal.behaviors.leanbox_dashboard_otif = {
		attach: function (context, settings) {
			
			var otif = Drupal.settings.leanbox_dashboard.otif;
      //google.charts.load('current', {'packages':['gauge','corechart', 'bar']});
			google.charts.setOnLoadCallback(function () {
				drawotifChart(otif);
			});

			$(".otif-submit").click(function (e) {

				e.preventDefault();
			  var start_date = $(this).parent().prev().prev().find("input[name='start_date[date]']").val();
				var end_date = $(this).parent().prev().find("input[name='end_date[date]']").val();
				var url = window.location.href;
				if (start_date != '' && end_date != '') {
					$.ajax({
						type: "POST",
						cache: false, //for Chrome and IE8
						url: "/chart-daterange-filter",
						data: {start_date: start_date, end_date: end_date, activity_type: 'otif', url: url},
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
          height: 300,
          redFrom: 0, redTo: 80,
          yellowFrom:80, yellowTo: 95,
					greenFrom:95, greenTo:100,
          minorTicks: 5,
					majorTicks: [0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100]
        };

        var chart = new google.visualization.Gauge(document.getElementById('otif_div'));

        chart.draw(data, options);
				
			}
		}
	};
})(jQuery);
