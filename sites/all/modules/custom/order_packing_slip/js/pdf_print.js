(function ($) {
  Drupal.behaviors.hours = {
    attach: function (context, settings) {
        alert('hi');
        $(window).load(function(){ window.print(); });
    }
  };
})(jQuery);
