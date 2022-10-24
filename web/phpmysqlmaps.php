<!DOCTYPE html >
  <head>
    <meta name="viewport" content="initial-scale=1.0, user-scalable=no" />
    <meta http-equiv="content-type" content="text/html; charset=UTF-8"/>
    <title>Rotas</title>
<link rel="stylesheet" type="text/css" href="estilo.css"/>
    <style>
      #map {
        height: 100%;
      }
      html, body {
        height: 100%;
        margin: 0;
        padding: 0;
      }
    </style>
  </head>
<html>
  <body>
          <div id="form">
            <br/>
        INSIRA A DATA EM QUE DESEJA OBTER A ROTA (Exemplo:2020-09-14) :
        <form id="formulario" action="phpmysqlmaps.php" method="post">
        <input type="text" name="dat"/><br/>   
        <input  id="Button" type="submit" value="Buscar"> <br/>       
<?php
require("conexBD.php");
$conn = mysqli_connect($servername, $username, $password, $database);

if((isset($_POST['dat']))){
$dat  = $_POST['dat'];
echo "Data de referência: ";
echo $dat;
$sql ="INSERT INTO `buscas` (`dat`) VALUES ('$dat')";
if (mysqli_query($conn, $sql)) {
    
}
 else {
      echo "Erro:<br> " . $sql . "<br>" . mysqli_error($conn);
}
}else{
echo "Insira a data";
}
?>
        <hr/ color="#483D8B" size="1">
        </form>

  </div>
    <div id="map"></div>

    <script>
      var customLabel = {
}

        function initMap() {
        var map = new google.maps.Map(document.getElementById('map'), {
          center: new google.maps.LatLng(-10.198828 , -48.900661 ),
          zoom: 13
        });
        var infoWindow = new google.maps.InfoWindow;

          downloadUrl('result.php', function(data) {
            var xml = data.responseXML;
            var markers = xml.documentElement.getElementsByTagName('marker');
            Array.prototype.forEach.call(markers, function(markerElem) {
            var id = markerElem.getAttribute('id');
           var address = markerElem.getAttribute('address');
            var type = markerElem.getAttribute('type');
              var point = new google.maps.LatLng(
                  parseFloat(markerElem.getAttribute('lat')),
                  parseFloat(markerElem.getAttribute('lng')));

              var infowincontent = document.createElement('div');
              var strong = document.createElement('strong');
              strong.textContent = name
              infowincontent.appendChild(strong);
              infowincontent.appendChild(document.createElement('br'));

              var text = document.createElement('text');
              text.textContent = address
              infowincontent.appendChild(text);
              var icon = customLabel[type] || {};
              var marker = new google.maps.Marker({
                map: map,
                position: point,
                label: icon.label
              });
              marker.addListener('click', function() {
                infoWindow.setContent(infowincontent);
                infoWindow.open(map, marker);
              });
            });
          });
        }
      function downloadUrl(url, callback) {
        var request = window.ActiveXObject ?
            new ActiveXObject('Microsoft.XMLHTTP') :
            new XMLHttpRequest;
        request.onreadystatechange = function() {
          if (request.readyState == 4) {
            request.onreadystatechange = doNothing;
            callback(request, request.status);
          }
        };
        request.open('GET', url, true);
        request.send(null);
      }
      function doNothing() {}
    </script>
    <script defer
   src="https://maps.googleapis.com/maps/api/js?key=***************wyffnVaQUx8hBDeE5FxE&callback=initMap">
    </script>
  </body>
</html>

