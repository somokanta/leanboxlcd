/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
(function ($) {
    Drupal.behaviors.new_sku_wise_return = {
        attach: function (context, settings) {
            gs_changes();
            $(".actual-return-qty").blur(function () {
                var sku = $(this).attr('sku_code');
                var actual_return_qty = $(this).val();
                var to_be_return_qty = $(this).closest('tr').find('.to_be_return_qty').text();

                if (actual_return_qty != '') {
                    var pop_up_return_qty_sum = 0;
                    reson_empty = 0;
                    $(".pop-up-return-qty_" + sku).each(function () {
                        pop_up_return_qty_sum += parseInt($(this).val());
                        var return_qty = parseInt($(this).val());
                        var pop_up_reason_code = $(this).closest('tr').find('.pop-up-reason-code').val();
                        $("[id^=pop_up_bill_]").hide();
                        // In the pop up if Actual Return Qty is greater than zero and reason is empty than pop up will be open
                        if (return_qty > 0 && pop_up_reason_code == '') {
                            $('#pop_up_bill_' + sku).show();
                            reson_empty = 1;
                        }

                    });

                    $("[id^=pop_up_bill_]").hide().children().hide();
                    // Open the pop up if Actual Return Qty is not equal to To Be return qty or Actual Returned Qty at SKU level is not equal to sum of Actual Returned Qty at Bill Level
                    if ((parseInt(actual_return_qty) != parseInt(to_be_return_qty)) || (parseInt(pop_up_return_qty_sum) != parseInt(actual_return_qty)) || reson_empty == 1) {
                        $('#pop_up_bill_' + sku).show().children().show();
                    }
                }
            });
            $(".close-pop-up-button").click(function () {
                var sku = $(this).attr('sku_key');
                $('#pop_up_bill_' + sku).hide();
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

            Drupal.ajax.prototype.commands.claimed_short_to_approval = function (ajax, response, status)
            {
                //$(".approval_flag").each(function () {
                var key = response.selectedValue;
                var flag = $(".flag_data_" + key).val();
                //var key = $(".flag_data_"+ selectedvalue).attr('data');
                var bill_detail = $(".bill_no_details_" + key).attr('data-value');
                var decoded_detail = $.parseJSON(bill_detail);

                if (decoded_detail.res_cashier_short == '') {
                    decoded_detail.res_cashier_short = 0;
                }

                if (decoded_detail.res_godown_short == '') {
                    decoded_detail.res_godown_short = 0;
                }

                if (flag == 'yes') {
                    $(".res-godown-short_" + key).val(decoded_detail.res_godown_short);
                    $(".dispatch-qty_" + key).val(decoded_detail.dispatch_qty);
                    $(".res-cashier-short_" + key).val(decoded_detail.res_cashier_short);
                    $(".claimed-godown-short_" + key).val(decoded_detail.claimed_godown_short);

                    $(".approved-qty_" + key).val(decoded_detail.claimed_godown_short);

                    var final_godown_short = parseInt(decoded_detail.res_godown_short) + parseInt($(".approved-qty_" + key).val());
                    $(".res-godown-short_" + key).val(final_godown_short);

                    var final_dispatch_value = parseInt(decoded_detail.sum_net_sales_quantity) - parseInt($(".res-godown-short_" + key).val());
                    $(".dispatch-qty_" + key).val(final_dispatch_value);

                    $(".approved-qty_" + key).on('change', function () {
                        //if (parseInt(decoded_detail.claimed_godown_short) > parseInt($(".approved-qty_" + key).val())) {
                        var final_godown_short = parseInt(decoded_detail.res_godown_short) + parseInt($(".approved-qty_" + key).val());
                        $(".res-godown-short_" + key).val(final_godown_short);

                        var claim_diff_value = parseInt(decoded_detail.claimed_godown_short) - parseInt($(".approved-qty_" + key).val());
                        var final_cashier_short = parseInt(decoded_detail.res_cashier_short) + parseInt(claim_diff_value);
                        $(".res-cashier-short_" + key).val(final_cashier_short);

                        var final_dispatch_value = parseInt(decoded_detail.sum_net_sales_quantity) - parseInt($(".res-godown-short_" + key).val());
                        $(".dispatch-qty_" + key).val(final_dispatch_value);
                        // }
                        /*else if (parseInt(decoded_detail.claimed_godown_short) == parseInt($(".approved-qty_" + key).val())) {
                         alert("here");
                         var final_godown_short = parseInt(decoded_detail.res_godown_short) + parseInt($(".approved-qty_" + key).val());
                         $(".res-godown-short_" + key).val(final_godown_short);
                         
                         var final_dispatch_value = parseInt(decoded_detail.sum_net_sales_quantity) - parseInt($(".res-godown-short_" + key).val());
                         $(".dispatch-qty_" + key).val(final_dispatch_value);
                         
                         var claim_diff_value = parseInt(decoded_detail.claimed_godown_short) - parseInt($(".approved-qty_" + key).val());
                         var final_cashier_short = parseInt(decoded_detail.res_cashier_short) + parseInt(claim_diff_value);
                         $(".res-cashier-short_" + key).val(final_cashier_short);
                         }
                         else {
                         alert("Approved quantity can not be greater than claimed godown short quantity.");
                         }*/

                    });
                } else if (flag == 'no') {
                    $(".res-godown-short_" + key).val(decoded_detail.res_godown_short);
                    $(".dispatch-qty_" + key).val(decoded_detail.dispatch_qty);
                    $(".res-cashier-short_" + key).val(decoded_detail.res_cashier_short);
                    $(".claimed-godown-short_" + key).val(decoded_detail.claimed_godown_short);

                    var final_cashier_short = parseInt(decoded_detail.res_cashier_short) + parseInt(parseInt(decoded_detail.claimed_godown_short));
                    $(".res-cashier-short_" + key).val(final_cashier_short);
                    $(".approved-qty_" + key).val(0);
                    $(".approved-qty_" + key).attr("readonly", true);
                } else {
                    $(".res-godown-short_" + key).val(decoded_detail.res_godown_short);
                    $(".dispatch-qty_" + key).val(decoded_detail.dispatch_qty);
                    $(".res-cashier-short_" + key).val(decoded_detail.res_cashier_short);
                    $(".claimed-godown-short_" + key).val(decoded_detail.claimed_godown_short);

                    $(".approved-qty_" + key).val('');
                    $(".approved-qty_" + key).attr("readonly", true);
                }
                //});
            };
            function gs_changes() {
                $(".gs_approved_qty").on('change', function () {
                    var key = $(this).attr('data-key');
                    var flag = $(".flag_data_" + key).val();
                    //$(".error").length > 0 &&
                    if (key >= 0 && flag) {

                        //var key = $(".flag_data_"+ selectedvalue).attr('data');
                        var bill_detail = $(".bill_no_details_" + key).attr('data-value');
                        var decoded_detail = $.parseJSON(bill_detail);

                        if (decoded_detail.res_cashier_short == '') {
                            decoded_detail.res_cashier_short = 0;
                        }

                        if (decoded_detail.res_godown_short == '') {
                            decoded_detail.res_godown_short = 0;
                        }

                        if (flag == 'yes') {

                            //if (parseInt(decoded_detail.claimed_godown_short) > parseInt($(".approved-qty_" + key).val())) {
                            var final_godown_short = parseInt(decoded_detail.res_godown_short) + parseInt($(".approved-qty_" + key).val());
                            $(".res-godown-short_" + key).val(final_godown_short);

                            var claim_diff_value = parseInt(decoded_detail.claimed_godown_short) - parseInt($(".approved-qty_" + key).val());
                            var final_cashier_short = parseInt(decoded_detail.res_cashier_short) + parseInt(claim_diff_value);
                            $(".res-cashier-short_" + key).val(final_cashier_short);

                            var final_dispatch_value = parseInt(decoded_detail.sum_net_sales_quantity) - parseInt($(".res-godown-short_" + key).val());
                            $(".dispatch-qty_" + key).val(final_dispatch_value);
                            // }
                            /*else if (parseInt(decoded_detail.claimed_godown_short) == parseInt($(".approved-qty_" + key).val())) {
                             alert("here");
                             var final_godown_short = parseInt(decoded_detail.res_godown_short) + parseInt($(".approved-qty_" + key).val());
                             $(".res-godown-short_" + key).val(final_godown_short);
                             
                             var final_dispatch_value = parseInt(decoded_detail.sum_net_sales_quantity) - parseInt($(".res-godown-short_" + key).val());
                             $(".dispatch-qty_" + key).val(final_dispatch_value);
                             
                             var claim_diff_value = parseInt(decoded_detail.claimed_godown_short) - parseInt($(".approved-qty_" + key).val());
                             var final_cashier_short = parseInt(decoded_detail.res_cashier_short) + parseInt(claim_diff_value);
                             $(".res-cashier-short_" + key).val(final_cashier_short);
                             }
                             else {
                             alert("Approved quantity can not be greater than claimed godown short quantity.");
                             }*/


                        } else if (flag == 'no') {
                            $(".res-godown-short_" + key).val(decoded_detail.res_godown_short);
                            $(".dispatch-qty_" + key).val(decoded_detail.dispatch_qty);
                            $(".res-cashier-short_" + key).val(decoded_detail.res_cashier_short);
                            $(".claimed-godown-short_" + key).val(decoded_detail.claimed_godown_short);

                            var final_cashier_short = parseInt(decoded_detail.res_cashier_short) + parseInt(parseInt(decoded_detail.claimed_godown_short));
                            $(".res-cashier-short_" + key).val(final_cashier_short);
                            $(".approved-qty_" + key).val(0);
                            $(".approved-qty_" + key).attr("readonly", true);
                        } else {
                            $(".res-godown-short_" + key).val(decoded_detail.res_godown_short);
                            $(".dispatch-qty_" + key).val(decoded_detail.dispatch_qty);
                            $(".res-cashier-short_" + key).val(decoded_detail.res_cashier_short);
                            $(".claimed-godown-short_" + key).val(decoded_detail.claimed_godown_short);

                            $(".approved-qty_" + key).val('');
                            $(".approved-qty_" + key).attr("readonly", true);
                        }

                    }

                });
            }

        }
    };
}(jQuery));