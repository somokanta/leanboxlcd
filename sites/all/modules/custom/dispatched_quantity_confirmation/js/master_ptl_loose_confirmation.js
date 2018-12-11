/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
(function ($) {
    Drupal.behaviors.master_ptl_loose_confirmation = {
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
            
             $(".pplc-short-qty").on("keypress keyup blur paste", function (event) {
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
            $(".pplc-short-qty").on('change', function () {
                var pplc_short_qty = $(this).val();
//                var child_sku = $(this).attr('child-sku');
//                var child_qty = $(this).attr('child-qty');
//                var final_short_qty = 0;
//                var final_disp_qty = 0;
                var og_qty = $(this).closest('tr').find('.og_qty').text();
                var bill_num = $(this).closest('tr').find('.bill_num').text();
                //var short_qty = $(this).closest('tr').find('.short_qty').text();
                var to_be_dispatch_qty = og_qty - pplc_short_qty;
                $(this).closest('tr').find('.to-be-dispatch-qty').val(to_be_dispatch_qty);
                if ($(this).val() == '') {
                    $(this).closest('tr').find('.to-be-dispatch-qty').val('');
                }
                
//                if (child_sku != '' && child_qty != '') {
//                    $(this).addClass('sum_parent_shrt' +bill_num+'-'+ child_sku); //class added to calculate sum of parent sku.
//                    $(this).closest('tr').find('.to-be-dispatch-qty').addClass('sum_parent_dis' +bill_num+'-'+ child_sku)
//                    //sum of parent sku multiply by child sku qty to calculate final qty of parent sku in multiple bills 
//                    $(".sum_parent_shrt" + bill_num + '-' + child_sku).each(function () {
//                        final_short_qty += +$(this).val() * child_qty;
//                    });
//                    
//                    $(".sum_parent_dis" + bill_num + '-' + child_sku).each(function () {
//                        final_disp_qty += +$(this).val() * child_qty;
//                    });
//                    
//                    $('.psq-'+bill_num+'-'+child_sku).val(final_short_qty);
//                    $('.disp-'+bill_num+'-'+child_sku).val(final_disp_qty);
//                }
//                if ($(this).val() == '') {
//                        $('.psq-'+bill_num+'-' + child_sku).val("");
//                        $('.disp-'+bill_num+'-' + child_sku).val("");
//                }
            });
            
            //change bg color of child sku
//            $(".show-bills").each(function () {;
//                var child_sku = $(this).attr('child-sku');
//                 $('.skus-' + child_sku).closest('tr').addClass('child-sku');
//                 $(".child-sku").css("background-color", "#e2e1e1") 
//            });
            
            //make child readonly
//            $(".pplc-short-qty").each(function () {;
//                var child_sku = $(this).attr('child-sku');
//                var bill_num = $(this).closest('tr').find('.bill_num').text();
//                $('.psq-'+bill_num+'-'+child_sku).prop('readonly', true);
//                $('.disp-'+bill_num+'-'+child_sku).prop('readonly', true);
//                $('.msh-res-'+bill_num+'-'+child_sku).attr('readonly', true);
//            });
            
//            //short reason
//            $(".master-pplc-short-reason").on('change', function () {
//                var short_reason = $(this).val();
//                var child_sku = $(this).attr('child-sku');
//                var bill_num = $(this).closest('tr').find('.bill_num').text();
//                if (child_sku != '') {
//                    //remove child sku reason and show only parent sku reason in child
//                    $('.msh-res-'+bill_num+'-'+child_sku).find('option').remove().end().append('<option value="'+short_reason+'">'+short_reason+'</option>').val(short_reason);
//                    $('.msh-res-'+bill_num+'-'+child_sku).val(short_reason);
//                }
//            });
            
//             $(".put_qty").each(function () {
//                var child_sku_put = $(this).attr('child-sku');
//                var child_qty_put = $(this).attr('child-qty');
//                var final_qty_put = 0;
//                if (child_sku_put != '' && child_qty_put != '') {
//                    $(this).addClass('sum_parent_put' + child_sku_put); //class added to calculate sum of parent sku.
//
//                    //sum of parent sku multiply by child sku qty to calculate final qty of parent sku in multiple bills 
//                    $(".sum_parent_put" + child_sku_put).each(function () {
//                        final_qty_put += +$(this).val() * child_qty_put;
//                    });
////                    console.log(final_qty_put);
//                    $('.put-qty_' + child_sku_put).val(final_qty_put);
//                }
//            });
            
        }}
})(jQuery);

            