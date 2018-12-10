/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */


(function ($) {
    Drupal.behaviors.bill_wise_editing_cld = {
        attach: function (context, settings) {
            $(".show-bills").click(function () {
                var sku = $(this).attr('sku_code');
                $("#pop_up_bill_" + sku).each(function () {
                    $("#pop_up_bill_" + sku).show();
                });
            });

            $(".close-pop-up-button").click(function () {
                $("[id^=pop_up_bill_]").hide();
            });
            
             $(".cld-confirmed-qty").on("keypress keyup blur paste", function (event) {
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
            
            $(".cld-confirmed-qty").on('change', function () {
                var cld_confirmed_qty = $(this).val();
                var og_qty = $(this).closest('tr').find('.og_qty').text();
                $(this).closest('tr').find('.to-be-dispatch-qty').val(cld_confirmed_qty);
                if ($(this).val() == '') {
                    $(this).closest('tr').find('.to-be-dispatch-qty').val('');
                }
                if (og_qty == cld_confirmed_qty) {
                    $(this).closest('tr').find('.cld-short-reason').attr('disabled', true);
                }else{
                    $(this).closest('tr').find('.cld-short-reason').attr('disabled', false);
                }
            });
        }}
})(jQuery);