(function ($) {
    Drupal.behaviors.Reclicks = {
        attach: function (context, settings) {
            jQuery(".multiple_click a").click(function () {
                jQuery(this).click(function (e) {
                    //e.preventDefault();
                    return false;
                });
                return true;
            });
        }
    };
})(jQuery);