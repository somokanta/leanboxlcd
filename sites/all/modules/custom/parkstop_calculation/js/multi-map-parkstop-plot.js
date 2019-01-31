var marker = {};
(function ($) {

    $(document).ready(function () {

        var default_lat = 20.5937;
        var default_lng = 78.9629;
        var max_lat;
        var max_lng;
        var min_lat;
        var min_lng;
        var centre = new L.LatLng(default_lat, default_lng);
        var poly_ob1 = [];
        var poly_ob2 = [];

        map = new MapmyIndia.Map('map-container', {center: centre, zoomControl: true, hybrid: true, search: true});
        map.setView([default_lat, default_lng], 5);
        
      /*  L.Marker.setBouncingOptions({
            bounceHeight: 40, // height of the bouncing
            bounceSpeed: 50, // bouncing speed coefficient
            contractHeight : 1,
        });
*/
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
        function mapmyindia_number_on_marker($ss) {
            var tr = $ss.closest('tr');
            var checked_lat = tr.attr('data-lat');
            var checked_lng = tr.attr('data-lng');
            var sr = tr.attr('data-sr');
            
            tr.find('.cell-details').show();
            
            var icon_path = window.location.origin + '/sites/all/themes/leanbox/images/map-marker.png';
            var icon = L.divIcon({className: 'my-div-icon', html: "<img style='position:relative;width:30px;height:30px' src=" + icon_path + '><span style="position: absolute;left: 0.5em;right: 1em;top: 5px;bottom:3em;font-size:12px;font-weight:bold;width: 100px;color:black;display: inline-block;height: 17px;text-align: left;line-height: 17px;" class="my-div-span">' + (sr) + '</span>', iconSize: [10, 10], popupAnchor: [12, -10]});/*function that creates a div over a icon and display content on the div*/
            var postion = new L.LatLng(checked_lat, checked_lng);/*WGS lolatlongcation object*/


            if (marker[sr] === undefined) {
                
                var text = tr.find('.cell-address').html();
                
                var mk = addMarker(postion, icon, text);
                marker[sr] = {mk: mk};
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
        }


        /*function to remove  markers from map*/
        function mapmyindia_removeMarker($rr) {
            var tr = $rr.closest('tr');
            
            tr.find('.cell-details').hide();
            var sr = tr.attr('data-sr');
            if (marker[sr] !== undefined) {
                //console.log("sr"+marker[sr]);
                //console.log("srmk"+marker[sr].mk);
                map.removeLayer(marker[sr].mk);
                delete marker[sr];
            }
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



        $(document).on('click', '.park-stop-main .select-all .form-checkbox', function () {
            $("input:checkbox[name^=list_form_items]").each(function () {
                $(this).trigger("change");
            });
        });
        
        $(document).on('click', '.park-stop-child .select-all .form-checkbox', function () {
            $("input:checkbox[name^=list_form_items1]").each(function () {
                $(this).trigger("change");
            });
        });
        
        $(document).on('change', 'input[name^=list_form_items]', function () {
            if ($(this).prop('checked')) {
                mapmyindia_number_on_marker($(this));
                draw_poliline_van_tracking($(this));
                createPolygon($(this));
                check_respective_childstop($(this));
            } else {
                mapmyindia_removeMarker($(this));
                remove_poliline_van_tracking($(this));
                deletePolygon($(this));
                check_respective_childstop($(this));
            }
        });
       

        function draw_poliline_van_tracking($van_tracks){
            var pts1 = [];
            var pts2 = [];
            var $van_track = $van_tracks.closest('tr');
            var latlong = $van_track.attr("latlong");
            if (latlong !== undefined) {
                var result = latlong.split(" ");
                      
            var i;
            var poly1;
                for (i = 0; i < result.length; i++) {
                     var result1 = result[i].split(",");
                    /*array of wgs points*/
                    pts1.push(new L.LatLng(result1[0], result1[1]));
                    /*parameters of polyline*/
                    var poly1param = {
                        color: '#ff0000',
                        weight: 3,
                        opacity: 2,
                    };
                    var poly1 = new L.Polyline(pts1, poly1param);/*polyline with given points and default color is created*/
                    map.addLayer(poly1);
                    marker[latlong] = {i: poly1};
                    poly_ob1.push(poly1);

                };
            }      
        };
                /*function to remove  markers from map*/
        function remove_poliline_van_tracking($rr) {
            var $van_track = $rr.closest('tr');
            var latlongnew = $van_track.attr("latlong");
            var polylength = 0;
            if (latlongnew !== undefined) {
                polylength = poly_ob1.length;
                if (polylength > 0) {
                    for (var i = 0; i < polylength; i++) {
                        map.removeLayer(poly_ob1[i]);
                    }
                }
            }          
        }
        
     function createPolygon($van_tracks){
            var pts1 = [];
            var $van_track = $van_tracks.closest('tr');

            var poly3;
            var latlongchild = $van_track.attr("latlongchild");
            if (latlongchild !== undefined) {
                var resultnew = latlongchild.split(" ");
                for (var k = 0; k < resultnew.length; k++) {
                    var resultnew1 = resultnew[k].split(",");
                    /*array of wgs points*/

                    pts1.push(new L.LatLng(resultnew1[0], resultnew1[1]));
                    poly3 = new L.polygon(pts1, {color: "green", draggable: true});/**creates a polygon with colour: green*/
//                    poly3.on("click", function (e)
//                    {
//                      poly3.bindPopup("Hello Polygon").openPopup();
//                    });
                    map.addLayer(poly3);
                    marker[latlongchild] = {k: poly3};
                    poly_ob2.push(poly3);
                };

            }        
        };
        
        function check_respective_childstop($res) {
            var $final_res = $res.closest('tr');
            var data_sr = $final_res.attr("data-sr");
            var result = data_sr.split(" ");
            var final = result[2];


            $(".park_name-park-" + final).prev('td').find('.child-stop-click').each(function () {
                var result_child = $(this).closest('tr').attr('data-sr');
                if (result_child !== undefined) {
                    var final_result_child = result_child.split('.');
                    var final_result_aft = final_result_child[1];
                    var final_result_bef = final_result_child[0];
                    if ($final_res.find('.park-stop-click').prop("checked") == true) {
                        if (final == final_result_bef) {
                            if ($('[data-sr="' + final + '.' + final_result_aft + '"]').closest('tr').find('.child-stop-click').prop("checked") == false) {
                                $('[data-sr="' + final + '.' + final_result_aft + '"]').closest('tr').find('.child-stop-click').trigger('click');
                            }
                        }
                    } else {
                        if (final == final_result_bef) {
                            if ($('[data-sr="' + final + '.' + final_result_aft + '"]').closest('tr').find('.child-stop-click').prop("checked") == true) {
                                $('[data-sr="' + final + '.' + final_result_aft + '"]').closest('tr').find('.child-stop-click').trigger('click');
                            }
                        }
                    }
                }
            });
        }
        
        function deletePolygon($rr) {
            var $van_track = $rr.closest('tr');
            var latlongchild = $van_track.attr("latlongchild");
            var polylength = 0;
            if (latlongchild !== undefined) {
                polylength = poly_ob2.length;
                if (polylength > 0) {
                    for (var i = 0; i < polylength; i++) {
                        map.removeLayer(poly_ob2[i]);
                    }
                }
            }          
        }   
        
    })
}(jQuery));



(function ($) {
    Drupal.behaviors.selectableRows1 = {
        attach: function (context, settings) {


            Drupal.ajax.prototype.commands.confirm_clicked = function (ajax, response, status)
            {
                mapmyindia_removeMarker_on_confirm();
            };


            Drupal.ajax.prototype.commands.filter_clicked = function (ajax, response, status)
            {
                mapmyindia_removeMarker_on_confirm();

            };
            
             Drupal.ajax.prototype.commands.filter_click_ajax = function (ajax, response, status)
            {
                mapmyindia_remove_all_on_confirm();

            };


            function mapmyindia_removeMarker_on_confirm() {
                $.each(marker, function (key, valueObj) {
                    map.removeLayer(marker[key].mk);
                });
                marker = {};
            }
            
            function mapmyindia_remove_all_on_confirm() {
                if ($('.park-stop-click').prop('checked', true)) {
                  $('.park-stop-click').trigger( "click" )
                }
                 if ($('.child-stop-click').prop('checked', true)) {
                  $('.child-stop-click').trigger( "click" )   
                }
            }



        }


    };
}(jQuery));
