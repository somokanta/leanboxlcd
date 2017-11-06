(function ($) {
	Drupal.behaviors.leanbox_payment_collection = {
		attach: function (context) {
      $('.field-name-field-field-amnt-difference input').attr("readonly", true);
      $(".field-name-field-cash-amt input").change(function() {
        var cash_amt = $(this).val();
        var id = $(this).attr('id');
        var res = id.split('-');
        var nid = res[2];

        var bill_value = $('.bill_value-'+nid).text();     

        var prev_cash_amt = $('.prev_cash_amt-'+nid).text();
        console.log(prev_cash_amt);
        if(prev_cash_amt == '') {
          prev_cash_amt = 0;
        }

        var prev_chq_amt = $('.prev_ch_amt-'+nid).text();
        if(prev_chq_amt == '') {
          prev_chq_amt = 0;
        }
        
        var ch_amt = $('#edit-node-'+nid+'-field-collection-cheque-amt-und-0-value').val();
        if(ch_amt == '') {
          ch_amt = 0;
        }

        var total = parseInt(bill_value) - (parseInt(cash_amt) + parseInt(ch_amt) + parseInt(prev_cash_amt) + parseInt(prev_chq_amt));
        
        $('#edit-node-'+nid+'-field-field-amnt-difference-und-0-value').val(total);
      });
      
      $(".field-name-field-collection-cheque-amt input").change(function() {
        var chq_amt = $(this).val();
        var id = $(this).attr('id');
        var res = id.split('-');
        var nid = res[2];

        var bill_value = $('.bill_value-'+nid).text();

        var prev_cash_amt = $('.prev_cash_amt-'+nid).text();
        if(prev_cash_amt == '') {
          prev_cash_amt = 0;
        }

        var prev_chq_amt = $('.prev_ch_amt-'+nid).text();
        if(prev_chq_amt == '') {
          prev_chq_amt = 0;
        }
        
        var cash_amt = $('#edit-node-'+nid+'-field-cash-amt-und-0-value').val();
        if(cash_amt == '') {
          cash_amt = 0;
        }

        var total = parseInt(bill_value) - (parseInt(cash_amt) +  parseInt(chq_amt) + parseInt(prev_cash_amt) + parseInt(prev_chq_amt));
      
        $('#edit-node-'+nid+'-field-field-amnt-difference-und-0-value').val(total);
      });   
  	}
	};
})(jQuery);
