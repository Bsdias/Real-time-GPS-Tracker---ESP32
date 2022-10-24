

#include <SoftwareSerial.h>
#include <Wire.h>

SoftwareSerial mySerial(10, 11);            // RX, TX Pins
String apn = "claro.com.br";                       //APN
String apn_u = "claro";                     //APN-Username
String apn_p = "claro";                     //APN-Password
String url = "rastreador42.com.br/paginawebenvia.php";  //URL for HTTP-POST-REQUEST
String apiKeyValor = "***";   
String latitude = "00.0000";   //String for the second Paramter (e.g. Sensor2)
String longitude = "12.0000";
String velocidade = "25";
String httpRequestData;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
  delay(10000);
}

void loop() { // run over and over

    gsm_sendhttp(); //Start the GSM-Modul and start the transmisson
    delay(6000); //Wait one minute

}


void gsm_sendhttp() {
  
  
 
  mySerial.println("AT"); // TESTE (RESPOSTA DEVE SER "OK")
  runsl();
  delay(4000);

 // mySerial.println("ATI"); // iNFORMA  O NOME DO MÓDULO E A REVISÃO
//  runsl();
//  delay(100);
/*
mySerial.println("AT+CBC"); // rETORNA O VALOR DA TENSÃO DA BATERIA EM % E EM  MILIVOLTS
 runsl();
 delay(100);
 mySerial.println("AT+CREG?"); // VERIFICA SE ESTÁ CONECTADO EM ALGUMA REDE
 runsl();
 delay(100);
 mySerial.println("AT+CSQ"); // Retorna a intensidade do sinal. Valor máximo de 31. O valor mínimo para uma operação segura é 5.
 runsl();
 delay(100);  
 mySerial.println("AT+COPS?"); // Retorna a operadora a que você está conectado.
 runsl();
  delay(100); 
 mySerial.println("AT+COPS=?"); // Retorna a lista de operadoras sob alcance do módulo.
  runsl();
  delay(100); 
  */
  mySerial.println("AT+SAPBR=3,1,Contype,GPRS");
  runsl();
  delay(100);
  mySerial.println("AT+SAPBR=3,1,APN," + apn);
  runsl();
  delay(100);
 mySerial.println("AT+SAPBR=3,1,USER," + apn_u); //Comment out, if you need username
 runsl();
 delay(100);
 mySerial.println("AT+SAPBR=3,1,PWD," + apn_p); //Comment out, if you need password
 runsl();
 delay(1000);
  mySerial.println("AT+SAPBR =1,1"); // habilitar GPRS
  runsl();
  delay(2000);

 // mySerial.println("AT+SAPBR =1,1"); // habilitar GPRS
 // delay(2000);
  runsl();
  mySerial.println("AT+SAPBR=2,1");
  runsl();
  delay(2000);
  mySerial.println("AT+HTTPINIT"); // Iniciando serviço HTTP
  runsl();
  delay(1000);
  mySerial.println("AT+HTTPPARA=CID,1");
  runsl();
  delay(100);
 mySerial.println("AT+HTTPPARA=URL," + url);
 runsl();
 delay(100);
 mySerial.println("AT+HTTPPARA=CONTENT,application/x-www-form-urlencoded");
  runsl();
  delay(100);
 mySerial.println("AT+HTTPDATA=192,10000");
 runsl();
  delay(100);
  
  httpRequestData = "api_key=" + apiKeyValor + "&latitude=" + latitude+ "&longitude=" + longitude + "&velocidade=" + velocidade;
                          
  mySerial.println(httpRequestData);
  runsl();
  Serial.println ( httpRequestData );
  delay(10000);
  mySerial.println("AT+HTTPACTION=1");
  runsl();
  delay(5000);
  mySerial.println("AT+HTTPREAD");
  runsl();
  delay(100);
  mySerial.println("AT+HTTPTERM");
  delay(1000);
  runsl(); 


 mySerial.println("AT+SAPBR =0,1"); // desabilitar GPRS
 delay(2000);
 runsl();
  
}

//Print GSM Status
void runsl() {
  while (mySerial.available()) {
    Serial.write(mySerial.read());
  }

}
