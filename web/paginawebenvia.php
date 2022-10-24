<?php
require("conexBD.php");
$conn = mysqli_connect($servername, $username, $password, $database);
if (!$conn) {
    die("Falha na conexão: " . mysqli_connect_error());
}
echo "Banco de Dados Conectado<br><br>";
 
  
    $api_key_valor = "APIsenha";

$api_key = $latitude = $longitude = $velocidade = $datahora = "";


if ($_SERVER['REQUEST_METHOD'] == 'POST'){
    $api_key = test_input($_POST["api_key"]);
    if($api_key == $api_key_valor) {

        $latitude   = test_input($_POST["latitude"  ]);
        $longitude  = test_input($_POST["longitude" ]);
        $velocidade = test_input($_POST["velocidade"]);
        $datahora   = test_input($_POST["datahora"  ]);
   
        if ($conn->connect_error) {
            die("Connection failed: " . $conn->connect_error);
        }  
 $sql = "INSERT INTO `dados` (`Latitude`, `Longitude`, `Velocidade`,`Data_Hora`) 
     		VALUES ('$latitude', '$longitude', '$velocidade', '$datahora')";
        
        if ($conn->query($sql) === TRUE) {
            echo "Gravado com sucesso";
        } 
        else {
            echo "Error: " . $sql . "<br>" . $conn->error;
        }
        $conn->close();
    }
    else {
     echo "Key de verificação incorreta... ";
    } }
else {
    echo "Não houve inserção de dados via HTTP POST.";}

function test_input($data){
    $data = trim($data);
    $data = stripslashes($data);
    $data = htmlspecialchars($data);
    return $data; }
?>

