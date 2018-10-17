(function ($) {
    Drupal.behaviors.deliverycost_calculation = {
        attach: function (context, settings) {
            $(".trip-id").blur(function () {
                $(".vehicle_number").val('');
                $(".trip-dropdown").val('');
                $(".cashier_names").val('');
            });
            $(".vehicle_number").blur(function () {
                $(".trip-id").val('');
                $(".trip-dropdown").val('');
                $(".cashier_names").val('');
            });
            
             $(".cashier_names").blur(function () {
                $(".trip-id").val('');
                $(".vehicle_number").val('');
            });


        }


    };
}(jQuery));            