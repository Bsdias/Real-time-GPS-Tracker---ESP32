<!DOCTYPE html>
<html>
<?php
require("conexBD.php");
$conn = mysqli_connect($servername, $username, $password, $database);
$mysqli = new mysqli($servername, $username, $password, $database);
 $consulta ="SELECT * FROM dados order by id desc limit 10 ";
 $cons = $mysqli->query($consulta) or die ($mysqli->error);
?>
	<head>
		<title>Rastreador 42</title>
			<link rel="stylesheet" type="text/css" href="estilo.css"/>
			<meta http-equiv="content-Type" content="text/html; charset=UTF-8"> 
	</head>
	<body style="background-color:#708090;">
		<body>
		<div bgcolor="black"id="geral">
			<div  id="cab">
			<br/>
<br/>
Localização GPS (Últimos Dados Obtidos)
		  </div>
	<hr/ color="#483D8B" size="1">
<div id="centro">
	<table id ="tabela">
	    <tr id="linha1">
        <td>LATITUDE:</td>
        <td>LONGITUDE:</td>
        <td>VELOCIDADE<br/>(km/h):</td>
		<td>DATA/HORA:</td>
		<td>LINK:</td>
    </tr>
<?php 
while($dadosgps = $cons->fetch_array()){
?>		
    <tr>
 <td>
<?php
echo $dadosgps["Latitude"];
?></td>
<td>
<?php
echo $dadosgps["Longitude"];
?></td>
<td>
<?php
echo $dadosgps["Velocidade"];
?></td>
<td>
<?php
echo $dadosgps["Data_Hora"];
?>
</td>
    </tr>
	
<?php
}
?>	
<br/>
</table>

</div>
<br/>
<hr/ color="#483D8B" size="1">
        <br/> 
        <form id="formulario" action="phpmysqlmaps.php" method="post">
   PARA VISUALIZAR A ROTA NO MAPA 
        <input  id="Button2" type="submit" value="CLIQUE AQUI"> <br/> 
         
        <br/> 
        </form>
	<hr/ color="#483D8B" size="1">
			<pre>				
Versão de teste 2.6
B.S.D
				</pre>	
		</body>
		<?php
header("Refresh: 3");
if (!$conn) {
    die("Falha na Conexão.: " . mysqli_connect_error());
}
echo "Conectado ao Banco de Dados...";
?>
<br/>
<br/>
		
</html>		

