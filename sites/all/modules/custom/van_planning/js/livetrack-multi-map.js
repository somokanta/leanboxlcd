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

            // @todo remove below line from dev and live
            // icon_path = 'http://localhost/leanbox/sites/all/themes/leanbox/images/map-marker.png';
            var icon = L.divIcon({className: 'my-div-icon', html: "<img style='position:relative;width:35px;height:35px' src=" + icon_path + '><span style="position: absolute;left: 0.8em;right: 1em;top: 5px;bottom:3em;font-size:12px;font-weight:bold;width: 17px;color:black;display: inline-block;height: 17px;text-align: center;line-height: 17px;" class="my-div-span">' + (sr) + '</span>', iconSize: [10, 10], popupAnchor: [12, -10]});/*function that creates a div over a icon and display content on the div*/
            var postion = new L.LatLng(checked_lat, checked_lng);/*WGS location object*/
            

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

            //tr.find('.cell-details').hide();
            var sr = tr.attr('data-sr');
            if (marker[sr] !== undefined) {
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



        $(document).on('click', '.select-all .form-checkbox', function () {
            $("input:checkbox[name^=list_form_items]").each(function () {
                $(this).trigger("change");
            });
        });

        $(document).on('change', 'input[name^=list_form_items]', function () {
            if ($(this).prop('checked')) {
                mapmyindia_number_on_marker($(this));
                //draw_poliline_van_tracking($(this));
            } else {
                mapmyindia_removeMarker($(this));
                //remove_poliline_van_tracking($(this));
            }
        });

        function draw_poliline_van_tracking($van_tracks) {
            var pts1 = [];
            var $van_track = $van_tracks.closest('tr');
            var latlong = $van_track.attr("latlong");
            var result = latlong.split(" ");
            var i;
            var poly1;
            var randomColor = getRandomColor_forvan();
            for (i = 0; i < result.length; i++) {
                var result1 = result[i].split(",");
                /*array of wgs points*/
                pts1.push(new L.LatLng(result1[0], result1[1]));
                /*parameters of polyline*/
                var poly1param = {
                    color: randomColor,
                    weight: 4,
                    opacity: 0.5
                };
                var poly1 = new L.Polyline(pts1, poly1param);/*polyline with given points and default color is created*/
                map.addLayer(poly1);
                marker[latlong] = {i: poly1};

            }
            ;
        }
        ;
        function getRandomColor_forvan() {
            var letters = '0123456789ABCDEF';
            var color = '#';
            for (var i = 0; i < 6; i++) {
                color += letters[Math.floor(Math.random() * 16)];
            }
            return color;
        }
        /*function to remove  markers from map*/
        function remove_poliline_van_tracking($rr) {
            var tr = $rr.closest('tr');
            var latlong = tr.attr('latlong');
            if (marker[latlong] !== undefined) {
                var result = latlong.split(" ");
                for (i = result.length - 1; i >= 0; i--) {
                    map.removeLayer(marker[latlong].i);
                }
                delete marker[latlong];
            }
        }
    })
}(jQuery));



(function ($) {
    Drupal.behaviors.selectableRows1 = {
        attach: function (context, settings) {


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


