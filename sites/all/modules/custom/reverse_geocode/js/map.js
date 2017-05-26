
(function($) {
    $(document).ready(function() {
        
        var map = null;
        var marker;
        var default_lat = 20.5937;
        var default_lng = 78.9629;
        var centre = new L.LatLng(default_lat, default_lng);
        var url_result;
        map = new MapmyIndia.Map('map-container', {center: centre, zoomControl: true, hybrid: true, search:false});
        /*1.create a MapmyIndia Map by simply calling new MapmyIndia.Map() and passsing it at the minimum div object, all others are optional...
         2.all leaflet mapping functions can be called simply on the L object
         3.MapmyIndia may extend and in future modify the customised/forked Leaflet object to enhance mapping functionality for developers, which will be clearly documented in the MapmyIndia API documentation section.*/
        //var marker = L.marker(centre).addTo(map);/**--add marker at the centre of map--**/
        //marker.bindPopup('Hello World');

        addMarker(centre, '', "Draggable marker sample", true);


        function addMarker(position, title) {
            
            var icon_path = window.location.origin + '/sites/all/themes/leanbox/images/map-marker.png';
            
            var myIcon = L.divIcon({className: 'my-div-icon', html: "<img style='position:relative;width:35px;height:35px' src=" + icon_path + '>', iconSize: [10, 10], popupAnchor: [12, -10]});
            
            marker = new L.Marker(position, {draggable: true, title: title, icon: myIcon});
            //marker.bindPopup("Loading...");
            map.addLayer(marker);
            map.setView(marker.getLatLng(), 5);
        }

        function updateMarker(position, zoom) {

            zoom = zoom || 17;
            
            marker.setLatLng(position);
            marker.closePopup();
            marker.unbindPopup();
            
            var latLngs = [marker.getLatLng()];
            var markerBounds = new L.LatLngBounds(latLngs);
            map.fitBounds(markerBounds, {maxZoom: zoom});
        }

        marker.on('move', function(event) {
            var marker = event.target;
            var position = marker.getLatLng();

            $('input[name=lat]').val(position.lat);
            $('input[name=lng]').val(position.lng);
        });

        marker.on('click', function(event) {
            var marker = event.target;
            var position = event.target.getLatLng();
            var popup = marker.getPopup();
            
            if(popup == undefined){
                getUrlResult(position.lat, position.lng);/*get JSON resp*/
                marker.bindPopup(url_result);
                marker.openPopup();
            }
                    
            //popup.setContent(url_result).update()
        });
        
        marker.on('dragend', function(event) {
            var marker = event.target;
            marker.closePopup();
            marker.unbindPopup();
            
        });


//            function removeMarker() {
//                var markerlength = marker.length;
//                if (markerlength > 0) {
//                    for (var i = 0; i < markerlength; i++) {
//                        map.removeLayer(marker[i]);/* deletion of marker object from the map */
//                    }
//                }
//                delete marker;
//                marker = [];
//            }


        var GoogleSearch = L.Control.extend({
            options: {
                position: 'topleft'
            },
            onAdd: function() {

                this._map = map;
                this._container = L.DomUtil.create('div', 'leaflet-control-search');
                this._input = this._createInput('Search', 'search-input');
                this._cancel = this._createCancel('Cancel', 'search-cancel');

                return this._container;

            },
            cancel: function() {
                this._input.value = '';
                //this._handleKeypress({keyCode: 8});//simulate backspace keypress
                //this._input.size = this._inputMinSize;
                this._input.focus();
                this._cancel.style.display = 'none';
                //this._hideTooltip();
                return this;
            },
            _createInput: function(text, className) {
                var label = L.DomUtil.create('label', className, this._container);
                var input = L.DomUtil.create('input', className, this._container);
                input.type = 'text';
                input.autocomplete = 'off';
                input.autocorrect = 'off';
                input.autocapitalize = 'off';
                //input.style.display = 'none';
                input.role = 'search';
                input.id = 'searchBox';

                label.htmlFor = input.id;
                label.style.display = 'none';
                label.value = text;

                L.DomEvent
                        .disableClickPropagation(input)
                        .on(input, 'keydown', this._handleKeypress, this);

                return input;
            },
            _createCancel: function(title, className) {
                var cancel = L.DomUtil.create('a', className, this._container);
                cancel.href = '#';
                cancel.title = title;
                cancel.style.display = 'none';
                cancel.innerHTML = "<span>&otimes;</span>";//imageless(see css)

                L.DomEvent
                        .on(cancel, 'click', L.DomEvent.stop, this)
                        .on(cancel, 'click', this.cancel, this);

                return cancel;
            },
            _handleKeypress: function(e) {	//run _input keyup event

                switch (e.keyCode)
                {
                    case  8://Backspace
                    case 45://Insert
                    case 46://Delete
                        break;

                    default://All keys

                        if (this._input.value.length)
                            this._cancel.style.display = 'block';
                        else
                            this._cancel.style.display = 'none';
                }

                //this._handleAutoresize();
            },


        });

        (new GoogleSearch).addTo(map);
        var input = document.getElementById("searchBox");

        var searchBox = new google.maps.places.SearchBox(input);

        searchBox.addListener('places_changed', function() {
            
            var searchtext = input.value.trim();

            if (/^(\-?\d+(\.\d+)?),\s*(\-?\d+(\.\d+)?)$/.test(searchtext)) {
                var latlong = searchtext.split(',');
                var lat = latlong[0].trim();
                var long = latlong[1].trim();
                
                updateMarker([lat, long]);

            }
            else {
                var places = searchBox.getPlaces();
                if (places.length == 0) {
                    return;
                }

                var group = L.featureGroup();

                places.forEach(function(place) {

                    // Create a marker for each place.
                    console.log(places);
                    console.log(place.geometry.location.lat() + " / " + place.geometry.location.lng());
//                    var marker = L.marker([
//                        place.geometry.location.lat(),
//                        place.geometry.location.lng()
//                    ]);
//                    group.addLayer(marker);

                    updateMarker([
                        place.geometry.location.lat(),
                        place.geometry.location.lng()
                    ]);
                });

                //group.addTo(map);
                //map.fitBounds(group.getBounds()); 
            }




        });
        

        /*function to get Json response from the url*/
        function getUrlResult(lat, lng) {

            $.ajax({
                type: "GET",
                //dataType: 'text',
                url: "/reverse-geocode/get",
                async: false,
                data: {
                    lat: lat,
                    lng: lng
                },
                success: function(result) {
                    var jsondata = JSON.parse(result);

                    if (jsondata.responseCode == 200) {
                        display_rev_geocode_result(jsondata.results[0]);

                    }
                    /*handle the error codes and put the responses in divs. more error codes can be viewed in the documentation*/
                    else {
                        //document.getElementById('result').innerHTML = "No Result found";
                    }
                }
            });
        }

        /*function to display revgeocode result*/
        function display_rev_geocode_result(data)
        {
            var content = new Array();
//            if (data.city != '')
//                content.push('<tr><td style="white-space:nowrap;color:#222">City</td><td>' + data.city + '</td></tr>');
//            if (data.district != '')
//                content.push('<tr><td style="white-space:nowrap;color:#222">District</td><td>' + data.district + '</td></tr>');
//            if (data.state != '')
//                content.push('<tr><td style="white-space:nowrap;color:#222">state</td><td>' + data.state + '</td></tr>');
//            if (data.pincode != '')
//                content.push('<tr><td style="white-space:nowrap;color:#222">Pin</td><td>' + data.pincode + '</td></tr>');
            
//            if (data.houseNumber != '')
//                content.push('<tr><td style="white-space:nowrap;color:#222">House No.</td><td>' + data.houseNumber + '</td></tr>');
//
//            if (data.houseName != '')
//                content.push('<tr><td style="white-space:nowrap;color:#222">House Name</td><td>' + data.houseName + '</td></tr>');
            if (data.locality != '')
                content.push('<tr><td style="white-space:nowrap;color:#222">Locality</td><td>' + data.locality + '</td></tr>');
            if (data.subLocality != '')
                content.push('<tr><td style="white-space:nowrap;color:#222">subLocality</td><td>' + data.subLocality + '</td></tr>');
            if (data.street != '')
                content.push('<tr><td style="white-space:nowrap;color:#222">Street</td><td>' + data.street + '</td></tr>');
            if (data.poi != '')
                content.push('<tr><td style="white-space:nowrap;color:#222">Poi</td><td>' + data.poi + '</td></tr>');
//            if (data.poi_dist != '')
//                content.push('<tr><td style="white-space:nowrap;color:#222">Poi_dist</td><td>' + data.poi_dist + '</td></tr>');
//            if (data.street_dist != '')
//                content.push('<tr><td style="white-space:nowrap;color:#222">street_dist</td><td>' + data.street_dist + '</td></tr>');
//            if (data.village != '')
//                content.push('<tr><td style="white-space:nowrap;color:#222">Village</td><td>' + data.village + '</td></tr>');
//            if (data.formatted_address != '')
//                content.push('<tr><td style="white-space:nowrap;color:#222" valign="top">Formatted address</td><td width="10px" valign="top">:</td><td valign="top">' + data.formatted_address + '</td></tr>');
//            if (content == "")
//                content.push('<br>No details found for this place!!<br>');
              if (data.lat != '' && data.lng != '')
                content.push('<tr><td style="white-space:nowrap;color:#222">Lat / Lng</td><td>' + data.lat + ',' +data.lng + '</td></tr>');

            info_content = '<table style=\"width:306px;padding:10px;font-size: 12px;font-type: bold;\">' + content.join("") + '</table>';
            url_result = info_content;
        }



        $(document).on('change', 'input[name=list_form_items]', function() {

            var tr = $(this).closest('tr');
            var lat = tr.attr('data-lat');
            var lng = tr.attr('data-lng');

            if(lat.length == 0 || lng.length == 0){
                lat = default_lat;
                lng = default_lng;
                
                updateMarker([lat, lng], 5);
            }
            else{
                updateMarker([lat, lng]);
            }
        });
    })
}(jQuery));