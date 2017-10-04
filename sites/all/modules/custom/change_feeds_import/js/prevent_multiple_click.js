(function ($) {
    Drupal.behaviors.Reclicks = {
        attach: function (context, settings) {
            jQuery(".multiple_click").click(function () {
                cosole.log("called0");
                jQuery(this).click(function (e) {
                    cosole.log("called1");
                    e.preventDefault();
                    cosole.log("called2");
                });
                return true;
            });
        }
    };
})(jQuery);