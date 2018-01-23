(function ($) {
    Drupal.behaviors.vrs = {
        attach: function (context, settings) {
            var total = 0;
            var signed_collected = 0;
            var cheque_collected = 0;
            var cash_collected = 0;
            var ret_difference = 0;
            var van_bill_value = 0;
            var cash_debit = 0;
            var inventory_detail = 0;
            var total_debit = 0;

            $(".Signed_Bills-collected").change(function () {
                var signed_Sum = parseFloat($('.Signed_Bills-Sum_signed_bills-val').attr('data-value'));
                signed_collected = parseFloat($('.Signed_Bills-collected').val());
                var signed_difference = signed_Sum - signed_collected;
                $('.Signed_Bills-difference').val(signed_difference);
                total = signed_collected + cheque_collected + cash_collected;
                if (total == NaN) {
                    total = '';
                }
                else {
                  $('.cash_debit').val(total);
                }
                
                // 
                _calculate_total_debit();
            });


            $(".Cheque_Bills-collected").change(function () {
                var cheque_Sum = parseFloat($('.Cheque_Bills-Sum_cheque_bills-val').attr('data-value'));
                cheque_collected = parseFloat($('.Cheque_Bills-collected').val());
                var cheque_difference = cheque_Sum - cheque_collected;
                $('.Cheque_Bills-difference').val(cheque_difference);
                total = signed_collected + cheque_collected + cash_collected;
                if (total == NaN) {
                    total = '';
                }
                else {
                    $('.cash_debit').val(total);
                }

                //
                _calculate_total_debit();
            });


            $(".Cash_Bills-collected").change(function () {
                var cash_Sum = parseFloat($('.Cash_Bills-Sum_cash_bills-val').attr('data-value'));
                cash_collected = parseFloat($('.Cash_Bills-collected').val());
                var cash_difference = cash_Sum - cash_collected;
                $('.Cash_Bills-difference').val(cash_difference);
                total = signed_collected + cheque_collected + cash_collected;

                if (total == NaN) {
                    total = '';
                }
                else {
                    $('.cash_debit').val(total);
                }
                //
                _calculate_total_debit();

            });

            $(".ret_collected").change(function () {
                var ret_app = parseFloat($('.ret_val').attr('data-value'));
                var ret_collected = parseFloat($('.ret_collected').val());
                ret_difference = ret_app - ret_collected;
                $('.ret_difference').val(ret_difference);
                $('.inventory_detail').val(ret_collected);
                
                //
                _calculate_total_debit();
            });


            function _calculate_total_debit() {
                van_bill_value = parseFloat($('.van_bill_value').val());
                cash_debit = parseFloat($('.cash_debit').val());
                inventory_detail = parseFloat($('.inventory_detail').val());
                total_debit = van_bill_value - (cash_debit + inventory_detail);

                $('.total_debit').val(total_debit);
            }

        }}
})(jQuery);