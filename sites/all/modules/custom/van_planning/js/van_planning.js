/**
 * @file
 * Common js file for van planning.
 *
 */

(function ($) {
    Drupal.behaviors.vpselectableRows = {
        attach: function (context, settings) {
            $("#move_to_select").keyup(function () {
                var vanname = '';
                vanname = $("#move_to_select").val();
                if (vanname.match(/\s/g)) {
                    $('#move_to_select_error').html('<h4 style="color:red; font-size:12px">Space not allowed in Van name</h4>');
                    //vanname.value=t.value.replace(/\s/g,'');
                } else {
                    $('#move_to_select_error').html('');
                }
            });
            
            $("#van-table-div .form-checkbox").change(function () {
                var van_bill_count = 0;
                var van_bill_value = 0;
                var van_bill_sumkg = 0;
                var van_bill_sumcft = 0;
                var check_count = 0;
              //Iterate loop for one class and all willbe handled as same row is there
                $(".van_bill_count").each(function () {
                    if ($(this).closest('tr').find('.form-checkbox').prop('checked')) {
                        check_count++;// increment at one place
                        van_bill_count += parseInt($(this).closest('tr').find('.van_bill_count').text());
                        van_bill_value += parseInt($(this).closest('tr').find('.van_bill_value').text());
                        van_bill_sumkg += parseInt($(this).closest('tr').find('.van_bill_sumkg').text());
                        van_bill_sumcft += parseInt($(this).closest('tr').find('.van_bill_sumcft').text());
                    }
                });

                $('.subtotal_bill_count').text(van_bill_count);
                $('.subtotal_bill_value').text(van_bill_value);
                $('.subtotal_bill_sumkg').text(van_bill_sumkg);
                $('.subtotal_bill_sumcft').text(van_bill_sumcft);

                if (check_count < 1) {
                    $('.subtotal_bill_count').text('');
                    $('.subtotal_bill_value').text('');
                    $('.subtotal_bill_sumkg').text('');
                    $('.subtotal_bill_sumcft').text('');
                }

            });      
                            
            Drupal.ajax.prototype.commands.scrolling_field = function (ajax, response, status)
            {
               var elmnt = document.getElementById("child_element_visibility");
               elmnt.scrollIntoView();
            };
        }
    }
}(jQuery));
