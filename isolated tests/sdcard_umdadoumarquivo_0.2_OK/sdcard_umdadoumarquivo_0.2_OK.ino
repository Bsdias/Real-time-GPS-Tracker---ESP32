// Para gravar ler os novos arquivos a solução é simnples, utilizar as dicas do SDTESTe exemplo do esp, para listas os arqauivos
// usar as founções de nome e de oppennextfile

//#include <EEPROM.h>
#include "FS.h"
#include "SD.h"
#include "SPI.h"

//#define EEPROM_SIZE 16
#define SD_CS 5
int IDleitura;
int Longitude = 0;
int Lat = 0;
int Vel = 0;
int loc = 0;
String httpRequestData;
String key = "dc5dcd5cd";
void setup(){  
Serial.begin(9600);
//EEPROM.begin(EEPROM_SIZE);

SD.begin(SD_CS);
  if(!SD.begin(SD_CS)){
  Serial.println("Falha na leitura do cartão");
  return;
  }
    else{
Serial.println("Iniciando o cartão de memória");
}
//loc = EEPROM.read(0);
lersd(SD,"/");
}
void loop(){

//sdcard();

}
void sdcard(){

File file = SD.open("/"+String(loc)+".txt"); 
if(!file){
 
    Serial.println("Arquivo inexistente");
    Serial.println("Criando Arquivo");
  
 //writeFile(SD,"/"+String(loc)+".txt", String(Longitude) + "," + String(Lat) + "," + String(Vel) + "\r\n");
httpRequestData = "api_key="+String(key)+"&latitude="+String(Lat)+"&longitude="+String(Longitude)+"&velocidade="+String(Vel)+"";
writeFile(SD,"/"+String(loc)+".txt",httpRequestData);
  
    }
    else{
      Serial.println("O arquivo já existe");
      loc = loc + 1;
     // EEPROM.write(0, loc);
      }
       file.close();

 
delay(500);
  
  Longitude = Longitude +1;
  Lat = Lat + 2;
  Vel = Vel + 3;
}
void lerarquivo(fs::FS &fs, const char * path){
    Serial.printf("Reading file: %s\n", path);

    File file = fs.open(path);
    if(!file){
        Serial.println("Failed to open file for reading");
        return;
    }
   
 String c = "";
 while (file.available())
{
  c += (char)file.read();
}
       Serial.println("Conteudo:");
       Serial.println(c);
    file.close();
}
void lersd(fs::FS &fs, const char*diretorio){
File root = fs.open(diretorio);
if(!root){
  Serial.println("Falha ao abrir o diretoório");
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
   file = root.openNextFile();
}
file.close();
}
void writeFile(fs::FS &fs,  String path, String message) {
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
void deleteFile(fs::FS &fs, String path){ 
  // exemplo de como chamar esta função: deleteFile(SD, "/filename.txt");
    Serial.printf("Deleting file: %s\n", path);
    if(fs.remove(path)){
        Serial.println("File deleted");
    } else {
        Serial.println("Delete failed");
    }
}
