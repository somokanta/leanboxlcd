(function ($) {
    Drupal.behaviors.vrs_confirmation = {
        attach: function (context, settings) {
            calculateRowSum();//Calculate short again
            cal_short_total();//Calculate short again
            calc_footer_total();//Calculate short again
            if ($('.error').is(':visible')) {
                bill_status_undeliver_filter();
            }
            $(".total_actual_cash_collected").click(function () {
                $('#note_denomination_popup').show();

            });
            $("#bill_number").keyup(function () {
                bill_filter();
            });
            $("#payment_outcome").on('change', function () {
                payment_outcome_filter();
            });
            $("#bill_status").on('change', function () {
                bill_status_filter();
            });
            $("#bank_name").on('change', function () {
                bank_name_filter();
            });

            $("#bill_status_undeliver").on('change', function () {
                bill_status_undeliver_filter();
            });


            $("#close_popup_btn").click(function () {
                $('#note_denomination_popup').hide();
            });
            $(".img_close_btn").click(function () {
                $("#close_popup_btn").trigger("click");
            });
            // close Button 
            $(".note_denominations_text").each(function () {
                $(this).keyup(function () {
                    calculate_denomination_total();

                });
            });
            function calculate_denomination_total() {
                var calculated_total_sum = 0;
                $(".note_denominations_text").each(function () {
                    var get_textbox_value = $(this).val();
                    var get_note_denomination = $(this).attr('data');
                    if ($.isNumeric(get_textbox_value) && $.isNumeric(get_note_denomination)) {
                        calculated_total_sum += parseFloat(get_note_denomination * get_textbox_value);
                    }
                });

                //var coins_value = parseFloat($(".coins").val());
                //calculated_total_sum = calculated_total_sum + coins_value;
                //$(".total_actual_cash_collected").val(calculated_total_sum);
                $("#deno_total").html('Total: ' + calculated_total_sum);
                calc_footer_total();
                // $('#note_denomination_popup').hide();

            }

            $("#close_button").click(function () {
                calculate_denomination_total();
            });

            $("#bill_details").on('input', '.actual_cash_collected,.actual_cheque_amt_collected,.signed_bill_collected,.short,.cashier_total', function () {
                // Actual cash 
                var calculated_total_sum = 0;
                $("#bill_details .actual_cash_collected").each(function () {
                    var get_textbox_value = $(this).val();
                    if ($.isNumeric(get_textbox_value)) {
                        calculated_total_sum += parseFloat(get_textbox_value);
                    }
                });
                //$(".total_actual_cash_collected").val(calculated_total_sum);
                // Actual Cheque collected
                calculated_total_sum = 0;
                $("#bill_details .actual_cheque_amt_collected").each(function () {
                    var get_textbox_value = $(this).val();
                    if ($.isNumeric(get_textbox_value)) {
                        calculated_total_sum += parseFloat(get_textbox_value);
                    }
                });
                $(".total_cheque_collected").val(calculated_total_sum);

                // signed_bill_collected
                calculated_total_sum = 0;
                $("#bill_details .signed_bill_collected").each(function () {
                    var get_textbox_value = $(this).val();
                    if ($.isNumeric(get_textbox_value)) {
                        calculated_total_sum += parseFloat(get_textbox_value);
                    }
                });
                $(".total_signed_bill_collected").val(calculated_total_sum);


            });

            $(".short_cal").each(function () {
                $(this).keyup(function () {
                    calculateRowSum();

                });

            });


            $("#footer_details").on('input', '.cashier_total', function () {
                calc_footer_total();
                //}

            });
            /*$(".short_cal" ).focus(function() {
             calculated_total_sum = 0;
             $(".cashier_total").each(function () {
             var get_textbox_value = $(this).val();
             if ($.isNumeric(get_textbox_value)) {
             calculated_total_sum += parseFloat(get_textbox_value);
             
             }
             });
             var total_cashier_short_value = parseFloat($('.total_cashier_debit_value').attr('data'));
             var total_s_register = parseFloat($('.total_cashier_debit_value').attr('total'));
             //if (calculated_total_sum>total_s_register) {
             //alert("Total to be collected should be greater than Total Cash + Total Cheque + Total Signed Bill + Total Short.")
             //}
             // else {
             var to_c_short = total_s_register - calculated_total_sum;
             to_c_short = to_c_short + total_cashier_short_value
             $(".total_cashier_debit_value").val(to_c_short);
             });*/


            function calculateRowSum()
            {
                $('#bill_details tr:has(td)').each(function () {
                    var sum = 0;
                    var cash_sum = 0;
                    $(this).find('td').each(function () {
                        sum += parseFloat($(this).find('.short_cal').val()) || 0;
                        cash_sum += parseFloat($(this).find('.calculate_short_without_cash').val()) || 0;
                    });
                    var to_be_colelcted = parseFloat($(this).find('.short').attr('data'));
                    var row_total = to_be_colelcted - sum;
                    row_total = row_total.toFixed(2);
                    row_total_previous_value = row_total;
                    row_total = Math.abs(row_total);
                    // Difference in RS 1  Short amt will be greater than RS 1
                    if (row_total <= 1 && row_total >= 0) {
                        row_total = 0;
                    }
                    else {
                        row_total = row_total_previous_value;
                    }
                     /*Calculate "Short" at Bill level only IF Actual Cash collected =0.
                    // If Actual Cash collected > 0 then Short at bill level should be = 0.*/
                    if(cash_sum>0) {
                        row_total = 0;
                    }

                    $(this).find('.short').val(row_total);

                });
            }

            // Reseting filters
            $("#trip-id").change(function () {
                $(".dispatch_date").val('');
                $(".trip-dropdown").val('');
                // $(".trip-dropdown").html("");
                $(".cashier_names").val('');
                // $(".cashier_name").html("");
            });

            $(".dispatch_date").change(function () {
                $("#trip-id").val('');

            });


            $('#footer_details').on('change', '.cashier_total', function () {
                calc_footer_total();
            });
            $('#bill_details').on('change', '.short_cal, .short', function () {
                cal_short_total();
                calc_footer_total();

            });

            function cal_short_total() {
                // short
                calculated_total_sum = 0;
                $("#bill_details .short").each(function () {
                    var get_textbox_value = $(this).val();
                    if ($.isNumeric(get_textbox_value)) {
                        calculated_total_sum += parseFloat(get_textbox_value);

                    }
                });
                calculated_total_sum = calculated_total_sum.toFixed(2);
                $(".short_collected").val(calculated_total_sum);

            }

            function calc_footer_total() {
                // short

                /*calculated_cash_total_sum = 0;
                 $("#bill_details .actual_cash_collected").each(function() {
                 var get_textbox_value = $(this).val();
                 if ($.isNumeric(get_textbox_value)) {
                 calculated_cash_total_sum += parseFloat(get_textbox_value);
                 }
                 });*/


                calculated_total_sum = 0;
                $(".cashier_total").each(function () {
                    var get_textbox_value = $(this).val();
                    if ($.isNumeric(get_textbox_value)) {
                        calculated_total_sum += parseFloat(get_textbox_value);

                    }
                });
                //calculated_total_sum = calculated_cash_total_sum+calculated_total_sum;
                total_cashier_short_value = parseFloat($('.total_cashier_debit_value').attr('data'));
                total_s_register = parseFloat($('.total_cashier_debit_value').attr('total_sregister'));
                asset_total = parseFloat($('.total_cashier_debit_value').attr('asset_total'));
                excess_total = parseFloat($('.total_cashier_debit_value').attr('excess_value'));
                // sum of claimed gs
                sum_claimed_gs = parseFloat($('.total_cashier_debit_value').attr('sum_claimed_gs'));
                //alert(excess_total);
                //if (calculated_total_sum>total_s_register) {
                //alert("Total to be collected should be greater than Total Cash + Total Cheque + Total Signed Bill + Total Short.")
                //}
                // else {
                to_c_short = (parseFloat(total_s_register) - parseFloat(calculated_total_sum)).toFixed(2);
                //console.log(total_s_register);
                // console.log(calculated_total_sum);
                $(".total_cashier_cash_short").val(to_c_short);
                initial_debit_value = total_cashier_short_value;
                debit_val = (parseFloat(initial_debit_value) + parseFloat(to_c_short) + parseFloat(asset_total) + parseFloat(sum_claimed_gs)- excess_total).toFixed(2) ;
                $(".total_cashier_debit_value").val(debit_val);
            }
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

            $(".deny_string").on("keypress keyup blur paste", function (event) {
                var that = this;
                if (event.type === "paste") {
                    if (event.which < 48 || event.which > 57) {
                        event.preventDefault();
                    }
                } else {
                    if (event.which < 48 || event.which > 57) {
                        event.preventDefault();
                    }
                }

            });

            function bill_filter() {
                var input, filter, table, tr, td, i;
                input = document.getElementById("bill_number");
                var select = $('#bill_status');
                select.val($('options:first', select).val());
                //var bank_name = $('#bank_name');
                //bank_name.val($('options:first', select).val());
                $('#bank_name').val('');

                filter = input.value.toUpperCase();
                table = document.getElementById("bill_details");
                tr = table.getElementsByTagName("tr");
                for (i = 0; i < tr.length; i++) {
                    td = tr[i].getElementsByTagName("td")[0];
                    if (td) {
                        if (td.innerHTML.toUpperCase().indexOf(filter) > -1) {
                            tr[i].style.display = "";
                        } else {
                            tr[i].style.display = "none";
                        }
                    }
                }
                bill_status_undeliver_filter();
            }
            function payment_outcome_filter() {
                var input, filter, table, tr, td, i;
                input = document.getElementById("payment_outcome");
                var select = $('#bill_status');
                select.val($('options:first', select).val());
                $('#bank_name').val('');
                //bank_name.val($('options:first', select).val());
                $("#bill_number").val('');
                filter = input.value.toUpperCase();
                table = document.getElementById("bill_details");
                tr = table.getElementsByTagName("tr");
                for (i = 0; i < tr.length; i++) {
                    td = tr[i].getElementsByTagName("td")[3];
                    if (td) {
                        if (td.innerHTML.toUpperCase().indexOf(filter) > -1) {
                            tr[i].style.display = "";
                        } else {
                            tr[i].style.display = "none";
                        }
                    }
                }
                bill_status_undeliver_filter();
            }

            function bill_status_filter() {
                var input, filter, table, tr, td, i, check_all;
                input = $("#bill_status").val();
                $("#bill_number").val('');
                //var select = $('#bank_name');
                //select.val($('options:first', select).val());
                $("#bank_name").val('');

                $("#bill_status_undeliver").prop('checked', false);
                if (input == 'deliver') {
                    filter = 'Delivered';
                    check_all = 0;
                } else if (input == 'partial_deliver') {
                    filter = 'Partial Delivered';
                    check_all = 0;
                } else if (input == 'partial_return') {
                    filter = 'Partial Returned';
                    check_all = 0;
                }
                 else if (input == 'return') {
                    filter = 'Returned';
                    check_all = 0;
                }else {
                    check_all = 1;
                }

                table = document.getElementById("bill_details");
                tr = table.getElementsByTagName("tr");
                for (i = 0; i < tr.length; i++) {
                    td = tr[i].getElementsByTagName("td")[1];
                    if (td) {
                        if (check_all == 0) {
                            if (td.innerHTML.indexOf(filter) === 0) {
                                tr[i].style.display = "";
                            } else {
                                tr[i].style.display = "none";
                            }
                        } else {
                            if (td.innerHTML.indexOf('Full Returned') !== 0) {
                                tr[i].style.display = "";
                            }
                        }
                    }
                }
                bill_status_undeliver_filter();
            }

            function bank_name_filter() {
                var input, filter, table, tr, td, i;
                input = document.getElementById("bank_name");
                var select = $('#bill_status');
                select.val($('options:first', select).val());
                $("#bill_number").val('');
                $('#payment_outcome').val('');
                filter = input.value.toUpperCase();
                table = document.getElementById("bill_details");
                tr = table.getElementsByTagName("tr");
                for (i = 0; i < tr.length; i++) {
                    td = tr[i].getElementsByTagName("td")[23];
                    if (td) {
                        //document.getElementById("tabela").rows[n].cells[n].getElementsByTagName('input')[0].value;
                        //var bank_input =td.getElementsByTagName('input')[0];
                        //console.log(bank_input);
                        //bank_name = bank_input.value; 
                        //if (bank_name.toUpperCase().indexOf(filter) > -1) {
                        //  tr[i].style.display = "";
                        //} else {
                        // }

                        if (td.innerHTML.toUpperCase().indexOf(filter, fromIndex = 0) === 0) {
                            tr[i].style.display = "";
                        } else {
                            tr[i].style.display = "none";
                        }
                    }
                }
                bill_status_undeliver_filter();
            }

            Drupal.ajax.prototype.commands.bill_status_undeliver_filters = function (ajax, response, status)
            {
                close_popup();
                bill_status_undeliver_filter();
            };
            function close_popup() {
                $(".close").trigger("click");
            }
            function bill_status_undeliver_filter() {
                var input, filter, table, tr, td, i;
                filter = 'Full Returned';
                table = document.getElementById("bill_details");
                if (table) {
                    tr = table.getElementsByTagName("tr");
                    for (i = 0; i < tr.length; i++) {
                        td = tr[i].getElementsByTagName("td")[1];
                        if (td) {
                            if ($("#bill_status_undeliver").prop('checked') == true) {
                                if (td.innerHTML.indexOf(filter) === 0) {
                                    tr[i].style.display = "";
                                }
                            } else {
                                if (td.innerHTML.indexOf(filter) === 0) {
                                    tr[i].style.display = "none";
                                }
                            }
                        }
                    }
                }
            }

        }

    };
}(jQuery));