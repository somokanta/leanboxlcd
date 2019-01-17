/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
(function ($) {
    Drupal.behaviors.confirm_bill_editing_status = {
        attach: function (context, settings) {
            $("#pop_up_bill_").show();
            $(".close-pop-up-button").click(function () {
                $("[id^=pop_up_bill_]").hide();
            });
            
            $(".reason_all").change(function () {
                var reason = $(this).val();
                 $(".reason_bills").val(reason);
            });
            Drupal.ajax.prototype.commands.confirm_bill_editing_status = function (ajax, response, status)
            {
                close_popup();
              };
            function close_popup() {
                $(".error").trigger("click");
            }
            $(".reset_form").click(function () {
              
                var url = Drupal.settings.baseUrl + '/confirm-bill-editing-status';
                window.location = url;
            });
        }
      }
})(jQuery);

            