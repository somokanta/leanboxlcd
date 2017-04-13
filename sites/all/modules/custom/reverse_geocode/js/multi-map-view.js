var marker = {};
var max_lat;
var max_lng;
var min_lat;
var min_lng;
(function ($) {

    $(document).ready(function () {

        var default_lat = 20.5937;
        var default_lng = 78.9629;

        var centre = new L.LatLng(default_lat, default_lng);

        map = new MapmyIndia.Map('map-container', {center: centre, zoomControl: true, hybrid: true, search: false});
        map.setView([default_lat, default_lng], 5);

        L.Marker.setBouncingOptions({
            bounceHeight: 40, // height of the bouncing
            bounceSpeed: 50, // bouncing speed coefficient
            contractHeight: 1,
        });







        /*function to remove  markers from map*/
        function mapmyindia_removeMarker($rr) {
            var tr = $rr.closest('tr');

            tr.find('.cell-details').hide();
            var sr = tr.attr('data-sr');
            map.removeLayer(marker[sr].mk);

            delete marker[sr];
        }








    })
}(jQuery));



(function ($) {
    Drupal.behaviors.selectableRows2 = {
        attach: function (context, settings) {

            Drupal.ajax.prototype.commands.view_filter_clicked = function (ajax, response, status)
            {
                mapmyindia_removeMarker_on_confirm();
                $("input:checkbox[name^=list_form_items]").each(function () {
                    console.log($(this), "thisssss");
                    mapmyindia_number_on_marker($(this));
                });
            };

            function mapmyindia_removeMarker_on_confirm() {
                $.each(marker, function (key, valueObj) {
                    map.removeLayer(marker[key].mk);
                });
                marker = {};
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

                var icon = L.divIcon({className: 'my-div-icon', html: "<img style='position:relative;width:35px;height:35px' src=" + "'https://maps.mapmyindia.com/images/1.png'>" + '<span style="position: absolute;left:1.5em;right: 1em;top:0.9em;bottom:3em; font-size:9px;font-weight:bold;width: 1px; color:black;" class="my-div-span">' + (sr) + '</span>', iconSize: [10, 10], popupAnchor: [12, -10]});/*function that creates a div over a icon and display content on the div*/
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


        }


    };
}(jQuery));


