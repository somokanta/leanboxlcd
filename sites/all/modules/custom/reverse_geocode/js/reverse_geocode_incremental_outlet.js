(function ($) {
    Drupal.behaviors.incremental = {
        attach: function (context, settings) {
            console.log("123445567657");
            $('incremental-outlet-confirm-data').hide();
            $('#incremental-outlet-confirm').click(function () {
                $('incremental-outlet-confirm-data').show();
            });
            $("input[name^='list_form_items']").change(function () {
                $('incremental-outlet-confirm-data').hide();
            });
        }

    };
}(jQuery));