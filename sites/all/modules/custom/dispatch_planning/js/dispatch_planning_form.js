jQuery(document).ready(function() {
    jQuery(document).on('click', '.header-link', function(e) {
        e.preventDefault();
        var a = jQuery(this).attr('id');
        var sort_order = jQuery(this).attr('data-sort');

        console.log(sort_order);
        
        if(!sort_order){
            sort_order ='asc';
        }
        var b = ':' + sort_order;
        var c = a + b;
        jQuery('.sort-select').val(c).trigger('change');
    });
});