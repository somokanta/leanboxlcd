jQuery(document).ready(function($){
    var find_date_picker = $(".date-padding").length;
    //alert(find_date_picker);

    if (find_date_picker) {
    	$(".date-padding").closest(".views-exposed-widget").addClass("date_field_parent");
    }

    jQuery(".explore_menu").click(function(){
    	$(".menu_overlay").toggle();
    	$(".menu_dropdown").stop(true,true).slideToggle();
        jQuery('li.expanded.dropdown a').removeAttr('data-toggle');
    });

     $(".menu_overlay").click(function(){
    	$(".menu_overlay").toggle();
    	$(".menu_dropdown").stop(true,true).slideUp();
    });
    
    jQuery(".search_test").on("click", function() {
        var e = jQuery.Event("keydown");
        e.altKey = true; 
        e.keyCode = 68;      
        jQuery(document).trigger(e);
    });

    $('.print-pdf').click(function (event){
        console.log("print");
        event.preventDefault();
        window.print();});

});
