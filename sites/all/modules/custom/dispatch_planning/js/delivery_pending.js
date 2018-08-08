(function ($) {
    Drupal.behaviors.delivery_pending = {
        attach: function (context, settings) {
           $(".dp_pending_reason_all").change(function () {
                var dp_reason = $(this).val();
                 $(".dp_reason_bills").val(dp_reason);
            });
            Drupal.ajax.prototype.commands.bill_status_delivery_pending = function (ajax, response, status)
            {
                close_popup();
              };
            function close_popup() {
                $(".error").trigger("click");
            }
            $(".reset_form").click(function () {
              
                var url = Drupal.settings.baseUrl + '/admin/dispatch-planning/delivery-pending';
                window.location = url;
            });
        
        }}
})(jQuery);

