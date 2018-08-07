(function($) {
    Drupal.behaviors.vpselectableRows = {
        attach: function(context, settings) {
                        
            var selectableRow = $("table.table-radio tbody", context).children();
            selectableRow.click(function() {
                                
                $this = $(this);
                $this.siblings().removeClass("selected-row");
                $this.addClass("selected-row");
                $this.find("input.selectable-row").prop("checked", true).trigger("change");
            });

            var tableRow = $("table.table-checkbox tbody", context).children();

            tableRow.find('a.cell-details').click(function() {
                 console.log('content');
                var sr = $(this).closest('tr').attr('data-sr');
                marker[sr].mk.openPopup();
                //marker[sr].mk.bounce(3);
            })
            
//            selectableRow.find('a.cell-details').hover(function() {
//                $(this).siblings('.table-responsive').find('.cell-hide').toggleClass('hooover');
//            })

            selectableRow.find('a.cell-details').mouseenter(function() {
                var content = $(this).siblings('.table-responsive').find('.cell-hide').html();
                console.log('content' + content);
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
