(function($) {
    Drupal.behaviors.selectableRowsInOutlet = {
        attach: function(context, settings) {
                        
            var selectableRow = $("table.table-radio tbody", context).children();
            selectableRow.click(function() {
                $this = $(this);
                $this.siblings().removeClass("selected-row");
                $this.addClass("selected-row");
            });

        }
    };
}(jQuery));


