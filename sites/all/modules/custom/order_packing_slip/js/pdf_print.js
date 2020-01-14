(function ($) {
  Drupal.behaviors.hours = {
    attach: function (context, settings) {
        $(window).load(function(){ window.print(); });
    }
  };
})(jQuery);
