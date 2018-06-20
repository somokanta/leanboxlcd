(function($) {
    Drupal.behaviors.vrs_confirmation = {
        attach: function(context, settings) {
            Drupal.ajax.prototype.commands.remove_close_button = function (ajax, response, status)
            {
                close_popup();
                
            };
            function close_popup() {
                $(".close" ).trigger( "click" );
            }
        }
    };
}(jQuery));