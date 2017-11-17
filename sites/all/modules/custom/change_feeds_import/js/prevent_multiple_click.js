(function ($) {
    Drupal.behaviors.Reclicks = {
        attach: function (context, settings) {
            jQuery(".multiple_click a").click(function () {
                if(jQuery("#referesh_warning").length == 1) {
                    jQuery("#referesh_warning").show();
                }
                jQuery(this).click(function (e) {
                    //e.preventDefault();
                    return false;
                });
                return true;
            });
        }
    };
})(jQuery);