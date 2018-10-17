(function ($) {
    Drupal.behaviors.deliverycost_calculation = {
        attach: function (context, settings) {
           $('.mode_of_contract').each(function () {
                mode_of_contract = $(this).val();
                if (mode_of_contract == 'Contracted') {
                    $(".cost_per_day").prop('readonly', true);
                    $(".cost_per_month").prop('readonly', false);
                 } else if (mode_of_contract == 'Market') {
                    $(".cost_per_month").prop('readonly', true);
                    $(".cost_per_day").prop('readonly', false);
                   
                }
                else {
                     $(".cost_per_month").prop('readonly', true);
                     $(".cost_per_day").prop('readonly', true);
                }

            });
           
            $("#trip-id").change(function () {
                $(".dispatch_date").val('');
                $(".trip-dropdown").val('');
            });

            $(".dispatch_date").change(function () {
                $("#trip-id").val('');

            });
            $('.readonly_ref').prop('readonly', true);
            $(".cost_per_month").blur(function () {
                cost_per_month = $(this).val();
                if (cost_per_month)
                    cost_per_day = parseFloat(cost_per_month / 30).toFixed(2);
                else
                    cost_per_day = '';
                $(".cost_per_day").val(cost_per_day);

            });
            $(".resource_cost_per_month").blur(function () {
                cost_per_month = $(this).val();
                if (cost_per_month)
                    cost_per_day = parseFloat(cost_per_month / 30).toFixed(2);
                $(".resource_cost_per_day").val(cost_per_day);

            });
            $(".mode_of_contract").change(function () {
                mode_of_contract = $(this).val();
                if (mode_of_contract == 'Contracted') {
                    $(".cost_per_day").val('');
                    $(".cost_per_month").val('');
                    $(".cost_per_month").prop('readonly', false);
                    $(".cost_per_day").prop('readonly', true);
                    //cost_per_month = $(".cost_per_month").val();
                    // if (cost_per_month)
                    //     cost_per_day = parseFloat(cost_per_month / 30).toFixed(2);
                    //$(".cost_per_day").val(cost_per_day);
                } else if (mode_of_contract == 'Market') {
                    $(".cost_per_month").val('');
                    $(".cost_per_day").val('');
                    $(".cost_per_month").prop('readonly', true);
                    $(".cost_per_day").prop('readonly', false);
                }

            });
            $(".allownumericwithoutdecimal").on("keypress keyup blur paste", function (event) {
                var that = this;

                //paste event 
                if (event.type === "paste") {
                    setTimeout(function () {
                        $(that).val($(that).val().replace(/[^\d].+/, ""));
                    }, 100);
                } else {

                    if (event.which < 48 || event.which > 57) {
                        event.preventDefault();
                    } else {
                        $(this).val($(this).val().replace(/[^\d].+/, ""));
                    }
                }

            });
            $(".allownumericwithdecimal").keydown(function (event) {


                if (event.shiftKey == true) {
                    event.preventDefault();
                }

                if ((event.keyCode >= 48 && event.keyCode <= 57) ||
                        (event.keyCode >= 96 && event.keyCode <= 105) ||
                        event.keyCode == 8 || event.keyCode == 9 || event.keyCode == 37 ||
                        event.keyCode == 39 || event.keyCode == 46 || event.keyCode == 190) {

                } else {
                    event.preventDefault();
                }

                if ($(this).val().indexOf('.') !== -1 && event.keyCode == 190)
                    event.preventDefault();
                //if a decimal has been added, disable the "."-button

            });
        }


    };
}(jQuery));            