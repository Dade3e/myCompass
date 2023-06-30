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

void readFileLine(fs::FS &fs, const char * path){
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

String split(String My_S, char split, int n_part){
  String sub_S = My_S;
  int elementi = 0;
  for(int i =0; i < strlen(sub_S.c_str()); i++ ) {
    if(char(sub_S[i]) == split)
      elementi ++;
  }
  for(int i = 0; i <= elementi;i++){
    int index = sub_S.indexOf(split);
    String new_s = sub_S.substring(0,index);
    if(i == n_part)
      return new_s;
    sub_S = sub_S.substring(index+1);
  }
  return "";
}

bool writeFile(fs::FS &fs, const char * path, const char * message){
  Serial.printf("Writing file: %s\n", path);

  File file = fs.open(path, FILE_WRITE);
  if(!file){
    Serial.println("Failed to open file for writing");
    return false;
  }
  if(file.print(message)){
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
  file.close();
  return true;
}

bool appendFile(fs::FS &fs, const char * path, const char * message){
  Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if(!file){
    Serial.println("Failed to open file for appending");
    return false;
  }
  if(file.print(message)){
      Serial.println("Message appended");
  } else {
    Serial.println("Append failed");
  }
  file.close();
  return true;
}
