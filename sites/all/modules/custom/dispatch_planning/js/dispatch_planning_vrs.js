(function ($) {
    Drupal.behaviors.vrs = {
        attach: function (context, settings) {
            var total = 0;
            var signed_difference = 0;
            var cheque_difference = 0;
            var cash_difference = 0;
            var ret_difference = 0;
            var van_bill_value = 0;
            var cash_debit = 0;
            var inventory_detail = 0;
            var total_debit = 0;

            $(".Signed_Bills-collected").change(function () {
                var signed_app = $('.Signed_Bills-app_signed_bills-val').attr('data-value');
                var signed_collected = $('.Signed_Bills-collected').val();
                signed_difference = signed_app - signed_collected;
                $('.Signed_Bills-difference').val(signed_difference);
                total = signed_difference + cheque_difference + cash_difference;
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
                var cheque_app = $('.Cheque_Bills-app_cheque_bills-val').attr('data-value');
                var cheque_collected = $('.Cheque_Bills-collected').val();
                cheque_difference = cheque_app - cheque_collected;
                $('.Cheque_Bills-difference').val(cheque_difference);
                total = signed_difference + cheque_difference + cash_difference;
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
                var cash_app = $('.Cash_Bills-app_cash_bills-val').attr('data-value');
                var cash_collected = $('.Cash_Bills-collected').val();
                cash_difference = cash_app - cash_collected;
                $('.Cash_Bills-difference').val(cash_difference);
                total = signed_difference + cheque_difference + cash_difference;

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
                var ret_app = $('.ret_val').attr('data-value');
                var ret_collected = $('.ret_collected').val();
                ret_difference = ret_app - ret_collected;
                $('.ret_difference').val(ret_difference);
                $('.inventory_detail').val(ret_difference);

            });


            function _calculate_total_debit() {
                van_bill_value = $('.van_bill_value').val();
                cash_debit = $('.cash_debit').val();
                inventory_detail = $('.inventory_detail').val();
                total_debit = van_bill_value - (cash_debit + inventory_detail);
                $('.total_debit').val(total_debit);
            }

        }}
})(jQuery);