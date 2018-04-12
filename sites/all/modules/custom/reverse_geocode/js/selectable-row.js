(function($) {
    Drupal.behaviors.selectableRows = {
        attach: function(context, settings) {
                        
            var selectableRow = $("table.table-radio tbody", context).children();

            selectableRow.click(function() {
                                
                $this = $(this);
                $this.siblings().removeClass("selected-row");
                $this.addClass("selected-row");
                $this.find("input.selectable-row").prop("checked", true).trigger("change");
            });

            
//            selectableRow.find('.cell-details a').hover(function() {
//                
//                $(this).parents('tr').unbind('click');
//            }, function() {
//                
//                $(this).parents('tr').click(function() {
//                    $this = $(this);
//                    $this.siblings().removeClass("selected-row");
//                    $this.addClass("selected-row");
//                    $this.find("input:radio").prop("checked", true).trigger("change");
//                });
//            });

//            $('.cell-tooltip').addClass('cell-hide');


            var tableRow = $("table.table-checkbox tbody", context).children();
            
//            var bounceTimeout;
//            tableRow.hover(function() {
//
//                sr = $(this).attr('data-sr');
//                bounceTimeout = setTimeout(function() {
//                    bounceMarker(sr);
//                }, 700);
//            }, hideTooltip);
//
//            function bounceMarker() {
//                marker[sr].mk.bounce(3);
//            }
//
//            function hideTooltip() {
//                clearTimeout(bounceTimeout);
//            }
            tableRow.find('a.cell-details').click(function() {
                var sr = $(this).closest('tr').attr('data-sr');
                marker[sr].mk.openPopup();
                //marker[sr].mk.bounce(3);
            })
            
//            selectableRow.find('a.cell-details').hover(function() {
//                $(this).siblings('.table-responsive').find('.cell-hide').toggleClass('hooover');
//            })

            selectableRow.find('a.cell-details').mouseenter(function() {
                var content = $(this).siblings('.table-responsive').find('.cell-hide').html();
                $(this).tooltipster({
                    content: content,
                    contentAsHTML: true,
                    functionAfter: function(origin) {
                        origin.tooltipster('destroy');
                    }
                }).tooltipster('show')
            })

        }
    };    
}(jQuery));


