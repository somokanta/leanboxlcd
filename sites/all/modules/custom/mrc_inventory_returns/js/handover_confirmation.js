(function($) {
    Drupal.behaviors.handover_confirmation = {
        attach: function(context, settings) {
          $(".handover-amt").on('input',function () {
            actual_cash_amt = parseInt($(this).attr('data'));
            handover_amt = parseInt($(this).val());
              
            petty_cash_amt = parseInt(actual_cash_amt - handover_amt);
            if(petty_cash_amt > 0) {
              $(".petty-cash-amt").val(petty_cash_amt); 
            }
            else {
              $(".petty-cash-amt").val(''); 
            }
          });
          
          // Multiple select with checkboxes for Vrs Id field
          $('.vrs-ids-select-list').fSelect();
          
          // Multiple select with checkboxes for Trip Id field
          $('.trip-ids-select-list').fSelect();
        }        
    };
}(jQuery));


