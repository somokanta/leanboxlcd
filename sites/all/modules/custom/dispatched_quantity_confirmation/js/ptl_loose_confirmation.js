(function ($) {
    Drupal.behaviors.ptl_loose_confirmation = {
        attach: function (context, settings) {
            $(".pplc-confirmed-qty, .pplc-confirmed-qty-ptl").on("keypress keyup blur paste", function (event) {
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
                        
            //change put qty values
//            $(".put_qty").each(function () {
//                var child_sku_put = $(this).attr('child-sku');
//                var child_qty_put = $(this).attr('child-qty');
//                var final_qty_put = 0;
//                var bill_num = $(this).closest('tr').find('.bill_num').text();
//                if (child_sku_put != '' && child_qty_put != '') {
//                    $(this).addClass('sum_parent_put'+ bill_num+'-'+ child_sku_put); //class added to calculate sum of parent sku.
//
//                    //sum of parent sku multiply by child sku qty to calculate final qty of parent sku in multiple bills 
//                    $(".sum_parent_put"+bill_num+'-' + child_sku_put).each(function () {
//                        final_qty_put += +$(this).val() * child_qty_put;
//                    });
////                    console.log(final_qty_put);
//                    $('.put-qty_' +bill_num+'-' + child_sku_put).val(final_qty_put);
//                }
//            });
            
            
            //on ready disable child row and its editable fields
//            $(".pplc-confirmed-qty-ptl").each(function () {
//                 var child_sku = $(this).attr('child-sku');
//                 var bill_num = $(this).closest('tr').find('.bill_num').text();
//                 $('.pcc-'+bill_num+'-' + child_sku).closest('tr').addClass('child-sku');
//                 $(".child-sku").css("background-color", "#e2e1e1")
//                 $('.pcc-'+bill_num+'-' + child_sku).prop('readonly', true);
//                 $('.dis-'+bill_num+'-' + child_sku).prop('readonly', true);
//                 $('.sh-res-'+bill_num+'-' + child_sku).attr('readonly', true);
//            });

            $(".pplc-confirmed-qty-ptl").focusout(function () {
                var confirmed_qty = $(this).val();
                var bill_sku = $(this).attr('bill_sku');
//                var child_sku = $(this).attr('child-sku');
//                var child_qty = $(this).attr('child-qty');
//                var final_qty = 0;
                var put_qty = $(this).closest('tr').find('.put_qty').val();
                //var bill_num = $(this).closest('tr').find('.bill_num').text();
                $('.to-be-dispatched-qty-ptl_' + bill_sku).val(confirmed_qty);
                if ($(this).val() == '') {
                    $(this).closest('tr').find('.to-be-dispatched-qty-ptl_' + bill_sku).val(put_qty);
                }
//                if (child_sku != '' && child_qty != '') {
//                    $(this).addClass('sum_parent' +bill_num+'-'+ child_sku); //class added to calculate sum of parent sku.
//                    
//                    //sum of parent sku multiply by child sku qty to calculate final qty of parent sku in multiple bills 
//                    $(".sum_parent" +bill_num+'-' +  child_sku).each(function () {
//                        final_qty += +$(this).val() * child_qty;
//                    });
//                    $('.pcc-'+bill_num+'-' + child_sku).val(final_qty);
//                    $('.dis-'+bill_num+'-' + child_sku).val(final_qty);
//                    
//                    if ($(this).val() == '') {
//                        var put_qty_child = $('.dis-'+bill_num+'-' + child_sku).closest('tr').find('.put_qty').val();
//                        $('.pcc-'+bill_num+'-' + child_sku).val("");
//                        $('.dis-'+bill_num+'-' + child_sku).val(put_qty_child);
//                    }
//                }
            });

//            $(".pplc-short-reason-ptl").on('change', function () {
//                var short_reason = $(this).val();
//                var bill_num = $(this).closest('tr').find('.bill_num').text();
//                var child_sku = $(this).attr('child-sku');
//                if (child_sku != '') {
//                    //remove child sku reason and show only parent sku reason in child
//                    $('.sh-res-'+bill_num+'-' + child_sku).find('option').remove().end().append('<option value="'+short_reason+'">'+short_reason+'</option>').val(short_reason);
//                    $('.sh-res-'+bill_num+'-' + child_sku).val(short_reason);
//                }
//            });
            
           //nonptl
           $(".pplc-confirmed-qty").on('change', function () {
           var confirmed_qty = $(this).val();    
           var bill_sku = $(this).attr('bill_sku');    
           $('.to-be-dispatched-qty_' + bill_sku).val(confirmed_qty);
                if ($(this).val() == '') {
                    $(this).closest('tr').find('.to-be-dispatched-qty_' + bill_sku).val('');
                }
           });

        }}
})(jQuery);


