/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
(function ($) {
    Drupal.behaviors.block_party_billing = {
        attach: function (context, settings) {
            $("#pop_up_bill_").show();
            $(".close-pop-up-button").click(function () {
                $("[id^=pop_up_bill_]").hide();
            });
        }
      }
})(jQuery);

            