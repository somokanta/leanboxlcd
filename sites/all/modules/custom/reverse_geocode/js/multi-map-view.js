var marker = {};
var data;
(function ($) {

    $(document).ready(function () {

        var default_lat = 20.5937;
        var default_lng = 78.9629;
        var max_lat;
        var max_lng;
        var min_lat;
        var min_lng;
        var centre = new L.LatLng(default_lat, default_lng);

        map = new MapmyIndia.Map('map-container', {center: centre, zoomControl: true, hybrid: true, search: true});
        map.setView([default_lat, default_lng], 5);

        L.Marker.setBouncingOptions({
            bounceHeight: 40, // height of the bouncing
            bounceSpeed: 50, // bouncing speed coefficient
            contractHeight: 1,
        });

        function addMarker(position, icon, text) {
            if (icon == '') {
                var mk = new L.Marker(position, {draggable: false});/*marker with a default icon and optional param draggable, title */
                //  mk.bindPopup(title);
            } else {
                var mk = new L.Marker(position, {icon: icon, draggable: false});/*marker with a custom icon */
                //   mk.bindPopup(title);
            }
            mk.bindPopup(text);

            map.addLayer(mk);
            return mk;
        }


        /*function to make number appear on marker*/
        function mapmyindia_number_on_marker(lat, lng, key, closet_tr, nid, area_hook, randomColor) {
            var tr = closet_tr;
            var checked_lat = lat;
            var checked_lng = lng;

            //  var icon_path = window.location.origin + '/sites/all/themes/leanbox/images/map-marker.png';
            //  var icon = L.divIcon({className: 'my-div-icon', html: "<img style='position:relative;width:35px;height:35px' src=" + icon_path + '>', iconSize: [10, 10], popupAnchor: [12, -10]});/*function that creates a div over a icon and display content on the div*/



            var icon = L.divIcon({className: 'my-div-icon', html: "<div class='pin'  style='width:30px;height:30px;position:absolute;border-radius:50% 50% 50% 0;background:" + randomColor + ";transform:rotate(-45deg);left:50%;top:50%;margin:-20px 0 0 -20px;'></div>"});
            var postion = new L.LatLng(checked_lat, checked_lng);/*WGS location object*/


            $.ajax({
                type: "GET",
                //dataType: 'text',
                url: "/get/address",
                data: {
                    nid: nid,
                },
                success: function (result) {
                    var text = result;
                    var mk = addMarker(postion, icon, text);
                    marker[nid] = {mk: mk};
                    if (checked_lat > max_lat || max_lat == undefined) {
                        max_lat = checked_lat;
                    }
                    if (checked_lat < min_lat || min_lat == undefined) {
                        min_lat = checked_lat;
                    }
                    if (checked_lng > max_lng || max_lng == undefined) {
                        max_lng = checked_lng;
                    }
                    if (checked_lng < min_lng || min_lng == undefined) {
                        min_lng = checked_lng;
                    }

                    mapmyindia_array_of_location_fit_into_bound();
                }
            });

        }


        /*function to remove  markers from map*/
        function mapmyindia_removeMarker(nid) {
            map.removeLayer(marker[nid].mk);
            delete marker[nid];
        }

        function mapmyindia_array_of_location_fit_into_bound() {
//                var latitudeArr = [22.1874049914];
//                var longitudeArr = [74.8388671875];
//                var maxlat = Math.max.apply(null, latitudeArr);
//                var maxlon = Math.max.apply(null, longitudeArr);
//                var minlat = Math.min.apply(null, latitudeArr);
//                var minlon = Math.min.apply(null, longitudeArr);
            var southWest = L.latLng(max_lat, max_lng);/*south-west WGS location object*/
            var northEast = L.latLng(min_lat, min_lng);/*north-east WGS location object*/
            var bounds = L.latLngBounds(southWest, northEast);/*This class represents bounds on the Earth sphere, defined by south-west and north-east corners.i.e Creates a new WGS bounds.*/
            map.fitBounds(bounds);/*Sets the center map position and level so that all markers is the area of the map that is displayed in the map area*/

        }



//        $(document).on('click', '.select-all .form-checkbox', function () {
//            $("input:checkbox[name^=list_form_items]").each(function () {
//                $(this).trigger("change");
//            });
//        });



        $(document).on('change', 'input[name^=list_form_items]', function () {

            $('#area-load').show();
            var closet_tr = $(this).closest("tr");
            var area_hook = closet_tr.attr('data-area');
            var sales_day = closet_tr.attr('data-day');
            var van_pl_group = closet_tr.attr('data-pl-grp');

            if ($(this).prop('checked')) {
                var checked = 1;
            } else {
                var checked = 0;
            }

            $.ajax({
                type: "GET",
                //dataType: 'text',
                url: "/get/lat-lng",
                data: {
                    area_hook: area_hook,
                    sales_day: sales_day,
                    van_pl_group: van_pl_group,
                },
                success: function (result) {
                    var data = JSON.parse(result);
                    custom_ajax_func(data, area_hook);
                },
                complete: function () {
                    $('#area-load').hide();
                },
            });

            function custom_ajax_func(data, area_hook) {

                if (Object.getOwnPropertyNames(data).length !== 0) {
                    //is empty
                    var randomColor = getRandomColor();
                    area_hook = area_hook.replace(/\s/g, '');
                    var area_css = '.' + area_hook;
                    $(area_css).css('background-color', randomColor);
                    $(area_css).width(50).height(10);
                }else{
                    randomColor = "";
                }


                $.each(data, function (key, value) {
                    var lat = value.field_hul_updated_lat_value;
                    var lng = value.field_hul_updated_long_value;
                    var nid = value.nid;

                    if (checked == 1) {
                        mapmyindia_number_on_marker(lat, lng, key, closet_tr, nid, area_hook, randomColor);
                    } else {
                        $(area_css).css("background-color", "");
                        $(area_css).height('');
                        $(area_css).width('');
                        mapmyindia_removeMarker(nid);
                        max_lat = max_lng = min_lat = min_lng = undefined;
                    }

                });
            }
            function getRandomColor() {
                var letters = '0123456789ABCDEF';
                var color = '#';
                for (var i = 0; i < 6; i++) {
                    color += letters[Math.floor(Math.random() * 16)];
                }
                return color;
            }




        });





    })
}(jQuery));



(function ($) {
    Drupal.behaviors.selectableRows1 = {
        attach: function (context, settings) {
            $('#area-load').hide();
//            var selectableRow = $("table.selectable-row tbody").children();
//            selectableRow.click(function() {
//                $this = $(this);
//                $this.siblings().removeClass("selected-row");
//                $this.addClass("selected-row");
//                $this.find("input:checkbox").prop("checked", true).trigger("change");
//            });
//            $('.cell-tooltip').addClass('cell-hide');
//            selectableRow.find('.cell-details a').hover(function() {
//                $(this).parent().siblings('.cell-tooltip').toggleClass('hooover');
//            });


            Drupal.ajax.prototype.commands.confirm_clicked = function (ajax, response, status)
            {
                mapmyindia_removeMarker_on_confirm();
            };


            Drupal.ajax.prototype.commands.filter_clicked = function (ajax, response, status)
            {
                mapmyindia_removeMarker_on_confirm();

            };


            function mapmyindia_removeMarker_on_confirm() {
                $.each(marker, function (key, valueObj) {
                    map.removeLayer(marker[key].mk);
                });
                marker = {};
            }




        }


    };
}(jQuery));


