
#include <TinyGPS++.h>
#include <Wire.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "FS.h"
#include "SD.h"
#include "SPI.h"
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64
#define SD_CS 5

const char* ssid          = "SSID";
const char* senha         = "****";
String servidor           = "http:/******.com.br/***.php"; 
String apiKeyValor        = "API_Key"; 
const char CONTENT_TYPE[] = "application/x-www-form-urlencoded";
String apn     = "claro.com.br";                       
String apn_u   = "claro";                     
String apn_p   = "claro";
String initsim = "";
String loc     = "";
String dathora = "";
String httpRequestData;// Armazena os dados para enviar ao servidor
uint8_t i;
uint8_t redeAtiva;    // Esta variável indica o estado da conexão 
uint8_t bat;
int controlebat = 32;

HardwareSerial gps_serial(1);
HardwareSerial sim_serial(2);

TinyGPSPlus gps;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup(){
gps_serial.begin(9600, SERIAL_8N1, 15, 4);
sim_serial.begin(115200, SERIAL_8N1, 16, 17);
    Serial.begin(9600);
    SD.begin(SD_CS);
       if(!SD.begin(SD_CS)){
          Serial.println("Falha na leitura do cartão");
            return;
          }else{
          Serial.println("Iniciando o cartão de memória");
}
       if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
         Serial.println(F("falha no diplay"));
        for(;;);
  }
          
  pinMode(32,OUTPUT);      
  pinMode(33,INPUT);
  wifiinit();
}
void loop (){

DADOS_GPS();

    
}
void DADOS_GPS     () {
  while (gps_serial.available() > 0){
    if (gps.encode(gps_serial.read())){
       Serial.print(F("Localizacao: ")); 
  if (gps.location.isValid())
  {
    
  Serial.print(gps.location.lat(), 6);
  Serial.print(F(","));
  Serial.print(gps.location.lng(), 6);
 config_display();
 display.print("Latitude :");
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
  config_display(); 
  display.println("Conectando...");
  display.println("");
  display.println("Loc. indisponivel!");
  display.display();
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
       datahora();
  if (gps.location.isValid())
  {

    
   datahora();
   delay(200);
    httpRequestData = "api_key=" + apiKeyValor + "&latitude=" + String(gps.location.lat(), 6) + "&longitude=" + String(gps.location.lng(), 6) 
    + "&velocidade=" + String(gps.speed.kmph()) + "&datahora="+ dathora+"";
   verificaBackup(SD,"/"); // Verifica se há arquivos de backup antes de enviar os dados mais recentes
  //sdcard();// Descomente para gravar os dados no cartão cd de maneira direta 
  //enviarDados(); // Descomentte para enviar os dados diretamente (sem verificar backup
  }
  else
  {
    Serial.println(F("Ainda não conectado ao GPS:DADOS INVÁLIDOS!"));
    delay(10000);
  }

    }
    bateria();
    }
  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    
    Serial.println(F("GPS desconectado."));
    bateria();
    config_display();
      display.println("GPS Desativado!");
      display.display();
    while(true);
  } 
}
void resposta      () {
  while (sim_serial.available()) {
    Serial.write(sim_serial.read());
  }
}
void datahora      () {
 // Como o horário do GPS está adiantado em exatamente 3horas em relaçãop ao horário de brasília
 // Esta função tem como objetivo fazer esta correção.
int hora;
   hora = int(gps.time.hour()) - 3;
  if(int(gps.time.hour()) == 02){
    hora = 23;
  }
   if(int(gps.time.hour()) == 01){
    hora = 22;
  }
   if(int(gps.time.hour()) == 00){
    hora = 21;
  }
  dathora = String(gps.date.year())+"-"+String(gps.date.month())+"-"+String(gps.date.day())+" "+String(hora)
    +":"+String(gps.time.minute())+":"+String(gps.time.second())+""; 
   // Serial.println("Data e Hora: "+dathora);
 }
void enviarDados   () {
  redeAtiva = 0;
  if(WiFi.status()== WL_CONNECTED){
  COM_WIFI();
  }else{
  COM_GPRS();
  wifiinit();
    }
    }
void COM_GPRS      () {
  sim_serial.println("AT"); // TESTE (RESPOSTA DEVE SER "OK")
  delay(1000);
  VerificaModulo();
  sim_serial.println("AT+SAPBR=3,1,Contype,GPRS");// iniciando modo GPRS
  resposta();
  delay(100);
  sim_serial.println("AT+SAPBR=3,1,APN," + apn); //apn
  resposta();
  delay(100);
  sim_serial.println("AT+SAPBR=3,1,USER," + apn_u); //usuário
  resposta();
  delay(100);
  sim_serial.println("AT+SAPBR=3,1,PWD," + apn_p); //senha
  resposta();
  delay(1000);
  sim_serial.println("AT+SAPBR =1,1"); // habilitar GPRS
  delay(2000);
  VerificaRede();
  sim_serial.println("AT+SAPBR=2,1"); // Exibe o endereço IP do módulo
  resposta();
  delay(2000);
  sim_serial.println("AT+HTTPINIT"); // Iniciando serviço HTTP
  resposta();
  delay(1000);
  sim_serial.println("AT+HTTPPARA=CID,1");
  resposta();
  delay(100);
  sim_serial.println("AT+HTTPPARA=URL," + servidor);
  resposta();
  delay(100);
  sim_serial.println("AT+HTTPPARA=CONTENT,application/x-www-form-urlencoded");
  resposta();
  delay(100);
  sim_serial.println("AT+HTTPDATA=192,1000");
  resposta();
  delay(100);                        
  sim_serial.println(httpRequestData);
  resposta();
  Serial.println ( httpRequestData );
  delay(1000);
  sim_serial.println("AT+HTTPACTION=1");
  delay(2000);
  resposta();
  sim_serial.println("AT+HTTPREAD");
  resposta();
  delay(100);
  sim_serial.println("AT+HTTPTERM");
  delay(1000);
  resposta(); 


  sim_serial.println("AT+SAPBR =0,1"); // desabilitar GPRS
  delay(1000);
  resposta();

//Fim da Configuração da rede Conexão GPRS
}
void COM_WIFI      () {
 if(WiFi.status()== WL_CONNECTED){
  redeAtiva = 1;
  Serial.println("Conectado a uma rede WiFI");
    HTTPClient http;
    http.begin(servidor);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded", "Method","POST");

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
   
    http.end();
    delay(5000); 
  }
  else {
    Serial.println("WiFi Desconectado");
  }
   
  // Fim da configuração WIFI
 
   }
void wifiinit      () {
  
  WiFi.begin(ssid, senha);
  Serial.println("Conectando...");
  delay(800);
  if(WiFi.status() != WL_CONNECTED) { 
    Serial.println("WiFi não conectado");
  }else{
  Serial.println("");
  Serial.println("Conectado a rede WiFi, com o seguinte endereco IP: ");
  Serial.println(WiFi.localIP());
  }
  }
void VerificaModulo() {
 initsim = "";
  if(sim_serial.available()){
    while (sim_serial.available()) {
         initsim =  sim_serial.readStringUntil('\n');  
         Serial.println("Inicio: "+initsim);
      }
    
    }else{
      Serial.println("Falha ao inicar o módulo, gravando no Cartão de Memória");
      initsim = "falha";
     // sdcard();
      }
  }
void VerificaRede  () {
   String res = "";
    while (sim_serial.available()) {
    res =  sim_serial.readStringUntil('\n');  
       //  Serial.println("Respotal por linha: "+res);
      }
      if (res == initsim){
      Serial.println("Conectado a uma rede: "+res);
      redeAtiva = 1;
      }else{
      Serial.println("Não conectado a uma rede, gavando no cartão de memória.");
      sdcard();
      }
      Serial.println("Resposta Final: "+res);
}
void sdcard        () {
  
  if((String(gps.location.lat(), 6)) != "0.000000" ){
  if (gps.date.isValid()){
      if (gps.time.isValid()){
    loc = String(gps.date.month())+"."+ String(gps.date.day())+"."+ String(gps.date.year())+"_"+String(gps.time.hour())
    +"."+String(gps.time.minute())+"."+String(gps.time.second())+"."+String(gps.time.centisecond())+""; 
    Serial.print("Novo arquivo:");
    Serial.println(loc);         
    }
  }else{
    Serial.print("ERRO ao criar nome para novo arquivo");
    }
File file = SD.open("/"+String(loc)+".txt"); 
if(!file){
 
    Serial.println("Arquivo inexistente");
    Serial.println("Criando Arquivo");
    datahora();
    httpRequestData = "api_key=" + apiKeyValor + "&latitude=" + String(gps.location.lat(), 6) + "&longitude=" + String(gps.location.lng(), 6) 
  + "&velocidade=" + String(gps.speed.kmph()) + "&datahora="+ dathora+"";

    writeFile(SD,"/"+String(loc)+".txt",httpRequestData);
  
    }else{
      Serial.println("O arquivo já existe");
      }
       file.close();

delay(500);
  }else{
    Serial.println("Dado Inválido, NÃO GRAVADO.");
    }
}
void bateria       () {
  int nivelbat;
  bat = map(analogRead(33), 0, 4095, 0,100);
// Os níveis de bateria servem para enviar ao servidor o estado da bateria, se necessário.
// OBS: Devido a maneira como a leitura é feita, e o funcionamento do sistema, é consideraddo bateria descarrecada com leitura inferior a 55%
  if((bat>=0) && (bat<55)){ //leitura de 55 = bateria com aproximadamente 5V
    nivelbat = 0;
    digitalWrite(controlebat,HIGH);
    }
  if((bat>54) && (bat<65)){ //leitura de 65 = bateria com aproximadamente 6V
    nivelbat = 1;
    digitalWrite(controlebat,HIGH);
    }
  if((bat>64) && (bat<81)){ //leitura de 81 = bateria com aproximadamente 7V
    nivelbat = 2;
    digitalWrite(controlebat,HIGH);
    }
  if((bat>80)&& ((bat<97))){ //leitura de 100 = bateria com aproximadamente 8V
    nivelbat = 3;
    digitalWrite(controlebat,HIGH);
    } 
  if (bat>96){
    digitalWrite(controlebat,LOW);
    
    }
     
  }
void config_display() {

  display.clearDisplay();
  display.setTextSize(0.8);
  display.setTextColor(WHITE);
  display.setCursor(1, 0); 

// Monitoramento da bateria--------------------  
// O monitoramento da bateria foi colocado dentro da função do display, para que o display sempre exiba esta informação
  bat = map(analogRead(33), 0, 4095, 0,100);
  display.print("Bateria: ");
  display.println(String(bat)+"%");
  display.println("Coordenadas:");
  display.display();
  display.setTextSize(1);
// -------------------------------------------
  }
void lerarquivo    (fs::FS &fs, const char * path)           {
    Serial.printf("Lendo arquivo: %s\n", path);

    File file = fs.open(path);
    if(!file){
        Serial.println("Falha na abertura para leitura");
        return;
    }
   
 String c = "";
 while (file.available())
{
  c += (char)file.read();
}
       httpRequestData = c;
       
       Serial.println("Conteudo:");
       Serial.println(c);
    file.close();
}
void lersd         (fs::FS &fs, const char*diretorio)        {
File root = fs.open(diretorio);
if(!root){
  Serial.println("Falha ao abrir o diretório");
  return;
  }
  if(!root.isDirectory()){
    Serial.println("Não e um diretótio");
    return;
    }
File file = root.openNextFile();
  while(file){
    lerarquivo(SD, file.name());
  if(file.isDirectory()){
    Serial.print("  DIR : ");
    Serial.println(file.name());
   } else {
    Serial.print("Arquivo: ");
    Serial.println(file.name());
   }
   enviarDados();
   if(redeAtiva == 1){
    // se a rede estiver ativa, deletar arquivo já enviado ao servidor
   deleteFile(SD, file.name()); // IMPORTANTE: CUIDADO AQUI, APAGAR ARQUIVOS
   file = root.openNextFile();
   }else{
    Serial.println("Falha ao deletar; rede indisponível");
    break;
    }
}
file.close();
}
void writeFile     (fs::FS &fs, String path, String message) {
    //Escrevend0/gravando no cartão SD
  Serial.printf("Escrevendo no arquivo: %s\n", path);
  File file = fs.open(path, FILE_WRITE);
  if(!file) {
    Serial.println("Falha ao abrir o arquivo para escrita");
    return;
  }
  if(file.print(message)) {
    Serial.println("Arquivo atualizado");
  } else {
    Serial.println("Erro na escrita.");
  }
  file.close();
}
void deleteFile    (fs::FS &fs, String path)                 { 
    Serial.printf("Deleting file: %s\n", path);
    if(fs.remove(path)){
        Serial.println("File deleted");
    } else {
        Serial.println("Delete failed");
    }
}
void verificaBackup(fs::FS &fs, const char*diretorio)        {
 File root = fs.open(diretorio);
 File file = root.openNextFile();
 int i = 0;
   while(file){
    i++;  
   file = root.openNextFile();
   if(i>3){
    break;
    }
    }
    if(i>1){
    Serial.println("Possui Backup!");
    lersd(SD,"/");
    }else{
    Serial.println("Não Possui Backup!");
    enviarDados();
    }
file.close();
}
