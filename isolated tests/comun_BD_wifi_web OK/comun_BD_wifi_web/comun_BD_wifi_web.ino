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
const char* servidor = "****.com.br/***.php"; 
String apiKeyValor = "****";


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

    http.begin(servidor);
    
   
    http.addHeader("Content-Type", "application/x-www-form-urlencoded", "Method","POST");
    
  
    String httpRequestData = "api_key=" + apiKeyValor + "&latitude=" + String(gps.location.lat(), 6)
                          + "&longitude=" + String(gps.location.lng(), 6) + "&velocidade=" + String(gps.speed.kmph()) +"";
    Serial.print("httpRequestData: "); 
    Serial.println(httpRequestData);
    
    int httpResponseCode = http.POST(httpRequestData);
        
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
  //envia uma requisi????o HTTP POST  a cada 3 segundos
  delay(3000);

  // Fim da configura????o WIFI
 

//....................................................................................................................................................
    
    }
    }

  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("GPS n??o detectado"));
    while(true);
  } 
  
}
