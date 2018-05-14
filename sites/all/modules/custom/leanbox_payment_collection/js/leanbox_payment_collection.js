(function ($) {
    Drupal.behaviors.leanbox_payment_collection = {
        attach: function (context) {
            $('.page-upload-collection-bill-signed .error-class').hide();

            $('.credit_note_value').change(function () {
                $this = $(this);
                var credit_note_value = $this.val();
                if (credit_note_value < 0) {
                    //alert(tobe_collected);
                    $('.error-class').show();
                    $('.error-class').html('<div class = "error">Credit Note value can not be negative</div>');
                } else {
                    $('.error-class').hide();
                }
                var unaccounted_value = $this.parent().parent().siblings().find('.unaccounted_value').val();
                var Billvalue = $this.parent().parent().siblings('.Billvalue-val').attr('data-value');
                var tobe_collected = Billvalue - credit_note_value - unaccounted_value;
                if (tobe_collected < 0) {
                    //alert(tobe_collected);
                    $('.error-class').show();
                    $('.error-class').html('<div class = "error">To Be collected value can not be negative</div>');
                } else {
                    $('.error-class').hide();
                }
                $this.parent().parent().siblings().find('.remaining_amt').val(tobe_collected);

            });
            $('.unaccounted_value').change(function () {
                $this = $(this);
                var unaccounted_value = $this.val();
                if (unaccounted_value < 0) {
                    //alert(tobe_collected);
                    $('.error-class').show();
                    $('.error-class').html('<div class = "error">Unaccounted value can not be negative</div>');
                } else {
                    $('.error-class').hide();
                }
                var credit_note_value = $this.parent().parent().siblings().find('.credit_note_value').val();
                var Billvalue = $this.parent().parent().siblings('.Billvalue-val').attr('data-value');
                var tobe_collected = Billvalue - credit_note_value - unaccounted_value;
                if (tobe_collected < 0) {
                    //alert(tobe_collected);
                    $('.error-class').show();
                    $('.error-class').html('<div class = "error">To Be collected value can not be negative</div>');
                } else {
                    $('.error-class').hide();
                }
                $this.parent().parent().siblings().find('.remaining_amt').val(tobe_collected);

            });

            $('.field-name-field-field-amnt-difference input').attr("readonly", true);
            $(".field-name-field-cash-amt , .field-name-field-collection-cheque-amt input").change(function () {
                //var cash_amt = $(this).val();
                var id = $(this).attr('id');
                var res = id.split('-');
                var nid = res[2];

                var bill_value = $('.bill_value-' + nid).text();

                var prev_cash_amt = $('.prev_cash_amt-' + nid).text();
                if (prev_cash_amt == '') {
                    prev_cash_amt = 0;
                }

                var prev_chq_amt = $('.prev_ch_amt-' + nid).text();
                if (prev_chq_amt == '') {
                    prev_chq_amt = 0;
                }

                var ch_amt = $('#edit-node-' + nid + '-field-collection-cheque-amt-und-0-value').val();
                if (ch_amt == '') {
                    ch_amt = 0;
                }
                var cash_amt = $('#edit-node-' + nid + '-field-cash-amt-und-0-value').val();
                if (cash_amt == '') {
                    cash_amt = 0;
                }

                var total = parseInt(bill_value) - (parseInt(cash_amt) + parseInt(ch_amt) + parseInt(prev_cash_amt) + parseInt(prev_chq_amt));
                if (total == NaN) {
                    total = '';
                }
                $('#edit-node-' + nid + '-field-field-amnt-difference-und-0-value').val(total);
            });

        }
    };
})(jQuery);
