#include <TinyGPS++.h>
#include <Wire.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64

const char* ssid     = "6X9:42";
const char* senha = "everybodylies";
const char* servidor = "http://192.168.1.6/paginawebenvia.php"; 
String apiKeyValor = "***";


HardwareSerial teste(1);
TinyGPSPlus gps;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup()
{
 

  teste.begin(9600, SERIAL_8N1, 16, 17);
  Serial.begin(9600);
  
    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("falha no diplay"));
    for(;;);
  }
  
  WiFi.begin(ssid, senha);
  Serial.println("Conectando");
  while(WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Conectado a rede WiFi, com o seguinte endereco IP: ");
  Serial.println(WiFi.localIP());


  
}

void loop()
{

  while (teste.available() > 0){
    if (gps.encode(teste.read())){
       Serial.print(F("Localizacao: ")); 
  if (gps.location.isValid())
  {

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  display.clearDisplay();
  Serial.print(gps.location.lat(), 6);
  Serial.print(F(","));
  Serial.print(gps.location.lng(), 6);
 display.print("Latitude:");
 display.println(gps.location.lat(), 6);
 display.println("  ");
 display.print("Longitude:");
 display.println(gps.location.lng(), 6);
 display.println("  ");
 display.display(); 
 Serial.print(F(" "));
 
  }
  else
  {
    Serial.print(F("invalido"));
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  display.clearDisplay();
    display.print("Conectando...");
  }

  Serial.print("Velocidade:");
 if(gps.speed.isValid()){
 Serial.print(F(","));
 Serial.print(gps.speed.kmph());
 display.print("Vel:");
 display.print(gps.speed.kmph());
 display.print("km/h");
 display.display(); 
  
 }

  Serial.print(F("  Data/Hora: "));
  if (gps.date.isValid())
  {
    Serial.print(gps.date.month());
    Serial.print(F("/"));
    Serial.print(gps.date.day());
    Serial.print(F("/"));
    Serial.print(gps.date.year());
  }
  else
  {
    Serial.print(F("invalido"));
  }

  Serial.print(F(" "));
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour());
    Serial.print(F(":"));
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(F(":"));
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
    Serial.print(F("."));
    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    Serial.print(gps.time.centisecond());
  }
  else
  {
    Serial.print(F("Invalido"));
  }

  Serial.println();


//..................................................................................................................................................

     // Configurando rede WIFI
    
 if(WiFi.status()== WL_CONNECTED){
    HTTPClient http;
    
    // Your Domain name with URL path or IP address with path
    http.begin(servidor);
    
    // Specify content-type header
    http.addHeader("Content-Type", "application/x-www-form-urlencoded", "Method","POST");
    
    // Prepare your HTTP POST request data
    String httpRequestData = "api_key=" + apiKeyValor + "&latitude=" + String(gps.location.lat(), 6)
                          + "&longitude=" + String(gps.location.lng(), 6) + "&velocidade=" + String(gps.speed.kmph()) + "";
    Serial.print("httpRequestData: "); 
    Serial.println(httpRequestData);
    
    // You can comment the httpRequestData variable above
    // then, use the httpRequestData variable below (for testing purposes without the BME280 sensor)
    //String httpRequestData = "api_key=tPmAT5Ab3j7F9&sensor=BME280&location=Office&value1=24.75&value2=49.54&value3=1005.14";

    // Send HTTP POST request
    int httpResponseCode = http.POST(httpRequestData);
     
    // If you need an HTTP request with a content type: text/plain
    //http.addHeader("Content-Type", "text/plain");
    //int httpResponseCode = http.POST("Hello, World!");
    
    // If you need an HTTP request with a content type: application/json, use the following:
    //http.addHeader("Content-Type", "application/json");
    //int httpResponseCode = http.POST("{\"value1\":\"19\",\"value2\":\"67\",\"value3\":\"78\"}");
        
    if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();
  }
  else {
    Serial.println("WiFi Desconectado");
  }
  //envia uma requisição HTTP POST  a cada 3 segundos
  delay(3000);

  // Fim da configuração WIFI
 

//....................................................................................................................................................
    
    }


    }

  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("GPS não detectado"));
    while(true);
  }
  

  
}
