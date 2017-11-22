(function ($) {
    Drupal.behaviors.Reclicks = {
        attach: function (context, settings) {
            jQuery("form").submit(function () {
                jQuery(this).submit(function (e) {
                    e.preventDefault();
                });
                return true;
            });
        }
    };
})(jQuery);