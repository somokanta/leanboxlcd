(function ($) {
    Drupal.behaviors.Reclicks = {
        attach: function (context, settings) {
            jQuery(".multiple_click a").click(function () {
                alert("called0");
                jQuery(this).click(function (e) {
                    alert("called1");
                    e.preventDefault();
                    alert("called2");
                });
                return true;
            });
        }
    };
})(jQuery);