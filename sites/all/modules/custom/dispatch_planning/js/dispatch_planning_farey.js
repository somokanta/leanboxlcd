(function ($) {
    Drupal.behaviors.farey = {
        attach: function (context, settings) {
            $("div.error-msg").hide();
            $(".form-submit").show();
            $('.sku_reason_list').prop('disabled', 'disabled');
//$(".sku_reason_list").each(function () {
//    
//}
            // Check Order Status partial_deliver  
            var order_status = $(".order_status_list").val();

            $(".actual_qty_value").each(function () {
                //$(this).parent().parent().next().find('select').prop('disabled', 'disabled'); 
                var actual_qty = $(this).val();
                var net_qty = $(this).closest('td').prev().attr('data-value');
                /*if (parseInt(actual_qty) > parseInt(net_qty)) {
                    $("div.error-msg").show();
                    $("div.error-msg").css({'color': 'red', 'font-size': '100%'});
                    $(".form-submit").hide();
                    $(this).parent().parent().next().find('select').prop('disabled', 'disabled');
                } else */
                if (parseInt(actual_qty) == 0 || isNaN(parseInt(actual_qty))) {
                    // alert('hi1');
                    $(this).parent().parent().next().find('select').prop('disabled', 'disabled');
                    //$(".form-submit").show();
                } else {
                    //  alert('hi2');
                    if (order_status == 'partial_deliver') {
                       // $("div.error-msg").hide();
                        //$(".form-submit").show();
                        $(this).parent().parent('td').next().find('select').removeAttr('disabled');
                    }
                }

            });

            if (order_status == 'partial_deliver') {
                $(".actual_qty_value").on('change', function () {
                    var actual_qty = $(this).val();
                    var net_qty = $(this).closest('td').prev().attr('data-value');
                    if (parseInt(actual_qty) > parseInt(net_qty)) {
                       // $("div.error-msg").show();
                        //$("div.error-msg").css({'color': 'red', 'font-size': '100%'});
                       // $(".form-submit").hide();
                        $(this).parent().parent().next().find('select').prop('disabled', 'disabled');
                    } else if (parseInt(actual_qty) == 0 || isNaN(parseInt(actual_qty))) {
                        $(this).parent().parent().next().find('select').prop('disabled', 'disabled');
                        //$(".form-submit").show();
                    } else {
                       // $("div.error-msg").hide();
                       // $(".form-submit").show();
                        $(this).parent().parent('td').next().find('select').removeAttr('disabled');
                    }

                });
            }

            if (order_status == 'undeliver') {
                $(".cashier_short_list").on('change', function () {
                    // alert('hi');
                    var cash_qty = $(this).val();
                    var dispatch_qty = $(this).closest('td').prev().prev().prev().attr('data-value');
                    var ret_qty = dispatch_qty - cash_qty;
                    $(this).closest('td').prev().prev().find('input').val(ret_qty);
                });
            }


        }}
})(jQuery);