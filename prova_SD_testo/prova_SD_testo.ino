#include "FS.h"
#include "SD.h"
#include "SPI.h"


void readFile(fs::FS &fs, const char * path){
  Serial.printf("Reading file: %s\n", path);

  File file = fs.open(path);
  if(!file){
    Serial.println("Failed to open file for reading");
    return;
  }

  Serial.println("Read from file: ");
  String line = "";
  while(file.available()){
    char c = file.read();
    if(c == '\n'){
      Serial.println(line);
      line = "";
    }else{
      line += c;
    }
  }
  file.close();
}

void writeFile(fs::FS &fs, const char * path, const char * message){
  Serial.printf("Writing file: %s\n", path);

  File file = fs.open(path, FILE_WRITE);
  if(!file){
    Serial.println("Failed to open file for writing");
    return;
  }
  if(file.print(message)){
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
  file.close();
}

void appendFile(fs::FS &fs, const char * path, const char * message){
  Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if(!file){
    Serial.println("Failed to open file for appending");
    return;
  }
  if(file.print(message)){
      Serial.println("Message appended");
  } else {
    Serial.println("Append failed");
  }
  file.close();
}

String readFileString(fs::FS &fs, const char * path){
  Serial.printf("Reading file: %s\n", path);

  File file = fs.open(path);
  if(!file){
    Serial.println("Failed to open file for reading");
    return "";
  }

  Serial.println("Read from file: ");
  String text = "";
  while(file.available()){
    text += char(file.read());
  }
  file.close();
  return text;
}

String readIndex(fs::FS &fs, const char * path){
  double index = 0;
  Serial.printf("Reading file: %s\n", path);

  File file = fs.open(path);
  if(!file){
    Serial.println("Failed to open file for reading");
    return "";
  }

  Serial.println("Read index: ");
  String line = "";
  while(file.available()){
    char c = file.read();
    if(c == '\n'){
      file.close();
      return (line);
      line = "";
      index ++;
    }else{
      line += c;
    }
  }
  file.close();
  return "0,DEFAULT";
  
}

String split(String My_S, char split, int n_part){
  String sub_S = My_S;
  int elementi = 0;
  for(int i =0; i < strlen(sub_S.c_str()); i++ ) {
    if(char(sub_S[i]) == split)
      elementi ++;
  }
  for(int i = 0; i < elementi;i++){
    int index = sub_S.indexOf(split);
    String new_s = sub_S.substring(0,index);
    if(i == n_part)
      return new_s;
    sub_S = sub_S.substring(index+1);
  }
  return "";
}


void setup(){
  Serial.begin(115200);
  if(!SD.begin(5)){
    Serial.println("Card Mount Failed");
    return;
  }
  uint8_t cardType = SD.cardType();

  if(cardType == CARD_NONE){
    Serial.println("No SD card attached");
    return;
  }

  Serial.print("SD Card Type: ");
  if(cardType == CARD_MMC){
    Serial.println("MMC");
  } else if(cardType == CARD_SD){
    Serial.println("SDSC");
  } else if(cardType == CARD_SDHC){
    Serial.println("SDHC");
  } else {
    Serial.println("UNKNOWN");
  }

  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("SD Card Size: %lluMB\n", cardSize);

  //writeFile(SD, "/hello.txt", "Hello ");
  //appendFile(SD, "/hello.txt", "World!\n");
  readFile(SD, "/coords.txt");


  String calib = readFileString(SD, "/calib.txt");
  
  Serial.println(calib);
  Serial.println(split(calib, '\n', 0).toInt());
  String index = readIndex(SD, "/index.txt");
  Serial.println(split(index, ',', 0));
  
}

void loop(){

}