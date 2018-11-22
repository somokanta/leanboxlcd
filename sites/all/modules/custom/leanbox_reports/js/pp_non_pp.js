(function ($) {
    Drupal.behaviors.leanbox_reports = {
        attach: function (context, settings) {
            $(".trip_id").blur(function () {
                $(".trip_creation_start_date").val('');
                $(".trip_creation_end_date").val('');

            });
            $(".trip_creation_start_date").blur(function () {
                $(".trip_id").val('');
            });
            $(".trip_creation_end_date").blur(function () {
                $(".trip_id").val('');
            });
        }


    };
}(jQuery));            