(function ($) {
    Drupal.behaviors.incremental = {
        attach: function (context, settings) {
             console.log("123");
            $('#incremental-outlet-confirm-data').hide();
            $('#incremental-outlet-confirm').click(function () {
                console.log("check clickeddd");
                $('#incremental-outlet-confirm-data').show();
            });
               $("input[name^='list_form_items']").click(function() {
                   console.log("radion clickeddd");
                console.log("radion");
                $('#incremental-outlet-confirm-data').hide();
            });
        }

    };
}(jQuery));