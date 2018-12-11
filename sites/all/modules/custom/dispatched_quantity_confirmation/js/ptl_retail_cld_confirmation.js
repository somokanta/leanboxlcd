(function ($) {
    Drupal.behaviors.ptl_retail_cld_confirmation = {
        attach: function (context, settings) {
            $(".cld-confirmed-qty").on("keypress keyup blur paste", function (event) {
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
              $(".cld-confirmed-qty").on('change', function () {
                var confirmed_qty = $(this).val();
                var bill_sku = $(this).attr('bill_sku');
                $('.to-be-dispatched-qty_' + bill_sku).val(confirmed_qty);
            });

        }}
})(jQuery);
