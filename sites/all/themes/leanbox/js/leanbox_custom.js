jQuery(document).ready(function($){
    var find_date_picker = $(".date-padding").length;
    //alert(find_date_picker);

    if (find_date_picker) {
    	$(".date-padding").closest(".views-exposed-widget").addClass("date_field_parent");
    }

});