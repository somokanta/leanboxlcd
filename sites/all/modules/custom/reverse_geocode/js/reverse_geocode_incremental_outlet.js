(function ($) {
    Drupal.behaviors.incremental = {
        attach: function (context, settings) {
            $('#incremental-outlet-confirm-data').hide();
            $('#incremental-outlet-confirm').click(function () {
                $('#incremental-outlet-confirm-data').show();
            });
               $("input[name^='list_form_items']").click(function() {
                console.log("radion");
                $('#incremental-outlet-confirm-data').hide();
            });
        }

    };
}(jQuery));