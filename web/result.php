<?php
require("conexBD.php");
function parseToXML($htmlStr)
{
$xmlStr=str_replace('<','&lt;',$htmlStr);
$xmlStr=str_replace('>','&gt;',$xmlStr);
$xmlStr=str_replace('"','&quot;',$xmlStr);
$xmlStr=str_replace("'",'&#39;',$xmlStr);
$xmlStr=str_replace("&",'&amp;',$xmlStr);
return $xmlStr;
$connection=mysqli_connect ($servername, $username, $password);
if (!$connection) {
  die('Não conectado: ' . mysqli_error());}
$db_selected = mysqli_select_db($connection, $database);
if (!$db_selected) {
  die ('Can\'t use db : ' . mysqli_error());
}

$mysqli = new mysqli($servername, $username, $password, $database);
$dat1 = "SELECT * FROM `buscas` order by id desc limit 1";
$cons = $mysqli->query($dat1) or die ($mysqli->error);
while($date = $cons->fetch_array()){
$dat = $date["dat"];

}
$query ="SELECT * FROM `dados` WHERE Data_Hora >='$dat 00:00:00' AND Data_Hora <= '$dat 23:59:59'";
$result = mysqli_query($connection, $query);
if (!$result) {
  die('Invalid query: ' . mysqli_error());
}

header("Content-type: text/xml");
echo "<?xml version='1.0' ?>";
echo '<markers>';
$ind=0;
while ($row = @mysqli_fetch_assoc($result)){
  echo '<marker ';
  echo 'lat="' . $row['Latitude'] . '" ';
  echo 'lng="' . $row['Longitude'] . '" ';
  echo 'data="' . $row['Data_Hora'] . '" ';
  echo '/>';
  $ind = $ind + 1;
}

echo '</markers>';

?>


