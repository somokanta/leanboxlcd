(function ($) {
    Drupal.behaviors.upload_signed_bill_cheque = {
        attach: function (context, settings) {
            if ($('.error').is(':visible')) {
                bill_status_undeliver_filter();
            }

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

            function bill_filter() {
                var input, filter, table, tr, td, i;
                input = document.getElementById("bill_number");
                var select = $('#bill_status');
                select.val($('options:first', select).val());
                //var bank_name = $('#bank_name');
                //bank_name.val($('options:first', select).val());
                $('#bank_name').val('');

                filter = input.value.toUpperCase();
                table = document.getElementById("bill_details_one");
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
                table = document.getElementById("bill_details_one");
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
                } else {
                    check_all = 1;
                }

                table = document.getElementById("bill_details_one");
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
