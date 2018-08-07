/**
 * @file
 * Common js file for van planning.
 *
 */

(function ($) {
    Drupal.behaviors.vpselectableRows = {
        attach: function (context, settings) {
            $("#move_to_select").keyup(function () {
                var vanname = '';
                vanname = $("#move_to_select").val();
                if (vanname.match(/\s/g)) {
                    $('#move_to_select_error').html('<h4 style="color:red; font-size:12px">Space not allowed in Van name</h4>');
                    //vanname.value=t.value.replace(/\s/g,'');
                } else {
                    $('#move_to_select_error').html('');
                }
            });
            Drupal.ajax.prototype.commands.scrolling_field = function (ajax, response, status)
            {
               var elmnt = document.getElementById("child_element_visibility");
               elmnt.scrollIntoView();
            };
        }
    }
}(jQuery));
