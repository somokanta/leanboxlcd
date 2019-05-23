(function ($) {
  Drupal.behaviors.hours = {
    attach: function (context, settings) {
      
      $( document ).ajaxComplete(function( event, request, settings ) {
        if(settings.extraData !== undefined && settings.extraData._triggering_element_name === 'hul_code'){
          if ($('.closed-day').prop("checked") == true) {
            $('.closed-day').trigger('click');
          }
          if ($('.closed-special-day').prop("checked") == true) {
            $('.closed-special-day').trigger('click');
          }
        }
      });
      
        var days = '';
        for (days = 0; days < 7; days++) {
            if ($('.outlet-checkbox-' + days).prop("checked") == false) {
                $('.from-to-time-' + days).hide();
                $('.open-close-' + days).text('Close');
            }
            if ($('.special-outlet-checkbox-' + days).prop("checked") == false) {
                $('.special-from-to-time-' + days).hide();
                $('.special-open-close-' + days).text('Close');
            }

        }
        
      $('[class^="working-checkbox"]').each(function () {
        $(this).click(function () {
          var classname = $(this).attr('class');
          var classarr = $(classname.split(' '));
          var firstclassarr = $(classarr[1].split('-'));
          var day = firstclassarr[2];
          outlet_working_days(day);
        });
      });
 
      $('[class^="special-working-checkbox"]').each(function () {
        $(this).click(function () {
          var classname = $(this).attr('class');
          var classarr = $(classname.split(' '));
          var firstclassarr = $(classarr[1].split('-'));
          var special_day = firstclassarr[3];
          outlet_special_working_days(special_day);
        });
      });

      function outlet_working_days(day) {
        $('.outlet-checkbox-' + day).change(function () {
          if ($(this).is(":checked")) {
            $('.from-to-time-' + day).show();
          } else {
            $('.from-to-time-' + day).hide();
          }
        });
        
        $('.outlet-checkbox-' + day).change(function () {
          if ($(this).is(":checked")) {
              $('.open-close-' + day).text('Open');
            } else {
              $('.open-close-' + day).text('Close');
            }
        });       
      }
      
      function outlet_special_working_days(special_day) {
        $('.special-outlet-checkbox-' + special_day).change(function () {
          if ($(this).is(":checked")) {
            $('.special-from-to-time-' + special_day).show();
          } else {
            $('.special-from-to-time-' + special_day).hide();
          }
        });
        
        $('.special-outlet-checkbox-' + special_day).change(function () {
          if ($(this).is(":checked")) {
              $('.special-open-close-' + special_day).text('Open');
            } else {
              $('.special-open-close-' + special_day).text('Close');
            }
        });       
      }
    }
  };
})(jQuery);