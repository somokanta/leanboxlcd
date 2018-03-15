(function ($) {
  Drupal.behaviors.farey = {
    attach: function (context, settings) {
      $("div.error-msg").hide();
      $('#edit-submit').show();
      $(".sku_reason_list").prop('disabled', 'disabled');

      // Check Order Status partial_deliver  
      var order_status = $(".order_status_list").val();

      if (order_status == 'partial_deliver') {
        $(".actual_qty_value").on('change', function () {
          var actual_qty = $(this).val();
          var net_qty = $(this).closest('td').prev().attr('data-value');
          if (actual_qty > net_qty) {
            $("div.error-msg").show();
            $("div.error-msg").css({'color': 'red', 'font-size': '100%'});
            $('#edit-submit').hide();
            $(this).parent().parent().next().find('select').prop('disabled', 'disabled');
          }
          else {
            $("div.error-msg").hide();
            $('#edit-submit').show();
            $(this).parent().parent('td').next().find('select').removeAttr('disabled');
          }

        });
      }
    }}
})(jQuery);