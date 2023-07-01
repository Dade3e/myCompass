//KEYBOARD
String keyboard(String l){
  int x = 5;
  int y = 3;
  int exit = true;
  String testo = "";
  while(digitalRead(A) == LOW);
  while(digitalRead(B) == LOW);
  while(exit){
    if(digitalRead(UP) == LOW && y > 0)
      y -= 1;
    if(digitalRead(DOWN) == LOW && y < 3)
      y += 1;
    if(digitalRead(LEFT) == LOW && x > 0)
      x -= 1;
    if(digitalRead(RIGHT) == LOW && x < 10)
      x += 1;
    
    if(digitalRead(B) == LOW){
      exit = false;
      testo = "";
    }
  
    if(digitalRead(A) == LOW){
      char c = 'a';
      if(maiusc)
        c = keysM[y][x*2];
      else
        c = keys[y][x*2];
      if(c == '^'){
        maiusc = maiusc *-1 + 1;
      }else if(x >=9 && y == 3){
        exit = false;
        return testo;
      }else if(c == '<'){
        testo = testo.substring(0,strlen(testo.c_str()) -1);
      }else{
        testo += c;
      }
    }

    while(digitalRead(A) == LOW);
    while(digitalRead(B) == LOW);

    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.drawLine(0, 20, 128, 20, SSD1306_WHITE);
    display.setTextSize(1); // Draw 2X-scale text

    display.setCursor(1, 8);
    display.print(l);
    display.println(testo);

    if(maiusc){
      display.setCursor(1, 24);
      display.println(keysM[0]);
      display.setCursor(1, 34);
      display.println(keysM[1]);
      display.setCursor(1, 44);
      display.println(keysM[2]);
      display.setCursor(1, 54);
      display.println(keysM[3]);
    }
    else {
      display.setCursor(1, 24);
      display.println(keys[0]);
      display.setCursor(1, 34);
      display.println(keys[1]);
      display.setCursor(1, 44);
      display.println(keys[2]);
      display.setCursor(1, 54);
      display.println(keys[3]);
    }
    
    if(x >=9 && y == 3){
      display.drawRect((9*12),(3*10)+22,20,11,SSD1306_WHITE);
    }else{
      display.drawRect((x*12),(y*10)+22,8,11,SSD1306_WHITE);
    }
    

    /*Serial.print(x);
    Serial.print(" ");
    Serial.print(y);
    Serial.print(" ");
    Serial.print(keys[y][x*2]);
    Serial.print(" ");
    Serial.println(maiusc);*/
    
    
    display.display(); // Update screen with each newly-drawn line
    delay(100);
  }
  return testo;
}

//NUMPAD
String Numpad(String l){
  int exit = true;
  int y = 0;
  int x = 0;
  String testo = "";
  while(digitalRead(A) == LOW);
  while(digitalRead(B) == LOW);
  while(exit){
    if(digitalRead(UP) == LOW && y > 0)
      y -= 1;
    if(digitalRead(DOWN) == LOW && y < 3)
      y += 1;
    if(digitalRead(LEFT) == LOW && x > 0)
      x -= 1;
    if(digitalRead(RIGHT) == LOW && x < 10)
      x += 1;
    
    if(digitalRead(B) == LOW){
      exit = false;
      testo = "";
    }
  
    if(digitalRead(A) == LOW){
      char c = num[y][x*2];
      if(x >=2 && y == 3){
        exit = false;
        return testo;
      }else if(c == '<'){
        testo = testo.substring(0,strlen(testo.c_str()) -1);
      }else{
        testo += c;
      }
    }

    while(digitalRead(A) == LOW);
    while(digitalRead(B) == LOW);

    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.drawLine(0, 20, 128, 20, SSD1306_WHITE);

    display.setTextSize(1);
    display.setCursor(1, 6);
    display.print(l);
    display.println(testo);

    display.setCursor(1, 24);
    display.println(num[0]);
    display.setCursor(1, 34);
    display.println(num[1]);
    display.setCursor(1, 44);
    display.println(num[2]);
    display.setCursor(1, 54);
    display.println(num[3]);
    
    if(x >=2 && y == 3){
      display.drawRect((2*12),(3*10)+22,20,11,SSD1306_WHITE);
    }else{
      display.drawRect((x*12),(y*10)+22,8,11,SSD1306_WHITE);
    }
    
    /*Serial.print(x);
    Serial.print(" ");
    Serial.print(y);
    Serial.print(" ");
    Serial.println(num[y][x*2]);*/
    
    display.display();
    delay(100);
  }
  return testo;
}

//MENU
int Menu(){
  int exit = true;
  display.clearDisplay();
  //display.drawRoundRect(0, 0, 16, display.height(), 3, SSD1306_WHITE);
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(4, 0);
  display.println("M");
  display.setCursor(4, 16);
  display.println("E");
  display.setCursor(4, 32);
  display.println("N");
  display.setCursor(4, 48);
  display.println("U");
  display.setTextSize(1);
  display.drawRoundRect(20, w*16, 106, 14, 3, SSD1306_WHITE);
  for(int i = 0; i<4; i++){
    display.setCursor(24, 16*i + 4);
    display.println(menu_list[selected + i]);
  }
  display.display();
  while(digitalRead(A) == LOW);
  while(digitalRead(B) == LOW);
  delay(100);
  int max = (sizeof(menu_list) / sizeof(menu_list[0]));
  while(exit){

    if(digitalRead(UP) == LOW){
      w -= 1;
      if(w < 0){
        selected --;
        w = 0;
      }
      
    }
    if(digitalRead(DOWN) == LOW){
      w += 1;
      if(w > 3){
        selected ++;
        w = 3;
      }
    }
      
    
    if(selected < 0){
      w = 3;
      selected = max - 4;
    }
      
      
    
    if(selected >= max - 3){
      selected = 0;
      w = 0;
    }
      
    //Serial.print(w);
    //Serial.print(" ");
    //Serial.println(selected);
    if(digitalRead(A) == LOW){
      exit = false;
      return w+selected;
    }
    if(digitalRead(B) == LOW){
      exit = false;
      w = 0;
      selected = 0;
    }
    while(digitalRead(A) == LOW);
    while(digitalRead(B) == LOW);

    display.clearDisplay();

    //display.drawRoundRect(0, 0, 16, display.height(), 3, SSD1306_WHITE);
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(4, 0);
    display.println("M");
    display.setCursor(4, 16);
    display.println("E");
    display.setCursor(4, 32);
    display.println("N");
    display.setCursor(4, 48);
    display.println("U");
    display.setTextSize(1);
    display.drawRoundRect(20, w*16, 106, 14, 3, SSD1306_WHITE);
    for(int i = 0; i<4; i++){
      display.setCursor(24, 16*i + 4);
      display.println(menu_list[selected + i]);
    }
    display.display();
   
    delay(100);
  }
  //Serial.print("EXIT  ");
  //Serial.println(w+selected);
  return (w+selected);
}


//SELECT POINT
int SelectPoint(){
  String puntiMomentanei[4];
  while(digitalRead(A) == LOW){delay(100);};
  while(digitalRead(B) == LOW){delay(100);};
  delay(100);

  File file = SD.open("/coords.txt");
  if(!file){
    Serial.println("Failed to open file for reading");
    return 0;
  }

  Serial.println("Read from file: ");
  int line_count = 0;
  int line_index = 0;
  int s = 0;
  w = 0;
  String salva = "";
  while (file.available()) {
    String line = file.readStringUntil('\n');
    puntiMomentanei[3] = puntiMomentanei[2];
    puntiMomentanei[2] = puntiMomentanei[1];
    puntiMomentanei[1] = puntiMomentanei[0];
    puntiMomentanei[0] = line;

    if(puntiMomentanei[3] != ""){
      int exit = true;
      while(exit && s == 0){

        if(digitalRead(UP) == LOW)
          w -= 1;
        if(digitalRead(DOWN) == LOW)
          w += 1;
          
        if(w > 3){
          exit = false;          
          w = 3;
        }
        if(w < 0){
          w = 0;
        }

        if(digitalRead(A) == LOW){
          exit = false;
          s = 1;
          salva = puntiMomentanei[3-w];
        }
        if(digitalRead(B) == LOW){
          exit = false;
          s = 2;
        }
        while(digitalRead(A) == LOW);
        while(digitalRead(B) == LOW);

        display.clearDisplay();

        //display.drawRoundRect(0, 0, 16, display.height(), 3, SSD1306_WHITE);
        display.setTextSize(1);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(4, 0);
        display.println("P");
        display.setCursor(4, 14);
        display.println("O");
        display.setCursor(4, 28);
        display.println("I");
        display.setCursor(4, 42);
        display.println("N");
        display.setCursor(4, 56);
        display.println("T");
        display.setTextSize(1);
        display.drawRoundRect(20, w*16, 106, 14, 3, SSD1306_WHITE);
        for(int i = 0; i<4; i++){
          display.setCursor(24, 16*i + 4);
          display.println(split(puntiMomentanei[3-i], ',', 0));
        }
        display.display();
      
        delay(100);
      }
    }
  }

  file.close();
  if(s == 0){
    return SelectPoint();
  }
  if(s == 1){
    TARGET_NAME = split(salva, ',', 0);
    TARGET_COORDS[0] = split(salva, ',', 1).toDouble();
    TARGET_COORDS[1] = split(salva, ',', 2).toDouble();
    String abc = ("1,"+ TARGET_NAME +','+ split(salva, ',', 1) +','+ split(salva, ',', 2) +','+ String(brigh));
    Serial.println(abc.c_str());
    writeFile(SD, "/index.txt", abc.c_str());
    Start(TARGET_NAME);
    delay(1000);
    return 1;
  }
  if(s == 2){
    return 1;    
  }

  return 0;
}



//WALK
void Walk(String dest, int azimuth ,int dist, int sat){
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  display.setTextSize(1);
  display.setCursor(1, 0);
  display.println(dest);

  display.setTextSize(3);
  display.setCursor(10, 20);
  display.println(azimuth);

  display.setTextSize(1);
  display.setCursor(1, 54);
  if(dist > 9999){
    display.print(dist/1000);
    display.print(".");
    display.print((dist%1000)/100);
    display.println(" Km");
  }else{
    display.print(dist);
    display.println(" m");
  }

  display.setTextSize(1);
  display.setCursor(86, 54);
  display.print("Sat: ");
  display.println(sat);

  display.setTextSize(1);
  display.setCursor(96, 0);
  display.println(battery_level);
  

  //display.drawCircle(clock_center_x, clock_center_y, raggio, SSD1306_WHITE);
  draw_arrow(azimuth);
  
  //display.setTextSize(1);
  //display.setCursor(clock_center_x-(raggio/2),clock_center_y-3);
  //display.println(azimuth);
  display.display();
  
}

void draw_arrow(int line){
  int y = ((raggio-1)*cos(pi-(2*pi)/360*line))+clock_center_y;
  int x = ((raggio-1)*sin(pi-(2*pi)/360*line))+clock_center_x;
  int y1 = ((raggioBase-1)*cos(pi-(2*pi)/360*(line+vert)))+clock_center_y;
  int x1 = ((raggioBase-1)*sin(pi-(2*pi)/360*(line+vert)))+clock_center_x;
  int y2 = ((raggioBase-1)*cos(pi-(2*pi)/360*(line-vert)))+clock_center_y;
  int x2 = ((raggioBase-1)*sin(pi-(2*pi)/360*(line-vert)))+clock_center_x;
  /*display.drawLine(clock_center_x,clock_center_y,x,y,SSD1306_WHITE);
  display.drawLine(clock_center_x,clock_center_y,x1,y1,SSD1306_WHITE);
  display.drawLine(clock_center_x,clock_center_y,x2,y2,SSD1306_WHITE);*/
  //display.drawLine(x1,y1,x,y,SSD1306_WHITE);
  //display.drawLine(x2,y2,x,y,SSD1306_WHITE);
  //display.drawLine(x1,y1,clock_center_x,clock_center_y,SSD1306_WHITE);
  //display.drawLine(x2,y2,clock_center_x,clock_center_y,SSD1306_WHITE);

  //
  display.fillTriangle(x1,y1,x2,y2,clock_center_x,clock_center_y, SSD1306_INVERSE);
  display.fillTriangle(x,y,x1,y1,x2,y2, SSD1306_INVERSE);
  display.display();
}

//set brightness
void setbrightness(){
  int exit = true;
  while(exit){
    display.clearDisplay();
    display.fillRoundRect(0, 64-(brigh/4), 16, (brigh/4), 3, SSD1306_WHITE);
    display.drawRoundRect(0, 0, 16, display.height(), 3, SSD1306_WHITE);
    
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(26, 20);
    display.println("Set brightness");
    display.display();
    if(digitalRead(UP) == LOW)
      brigh += 20;
    if(digitalRead(DOWN) == LOW)
      brigh -= 20;
    if(digitalRead(A) == LOW || digitalRead(B) == LOW)
      exit = false;
    if(brigh < 1)
      brigh = 1;
    if(brigh > 255)
      brigh = 255;
    display.ssd1306_command(0x81);
    display.ssd1306_command(brigh);
    //Serial.println(brigh);
    delay(100);
  }
  String index = readFileString(SD, "/index.txt");
  String ab = split(index, ',', 0) +','+ split(index, ',', 1) +','+ split(index, ',', 2) +','+ split(index, ',', 3) +','+ String(brigh);
  //Serial.println(ab.c_str());
  writeFile(SD, "/index.txt", ab.c_str());
}

//WAIT FIX
void waitFix(int sat){
  display.clearDisplay();
  switch(next_image){
    case 0:
    case 1:
      display.drawBitmap(8, 16, gps_wait1, 24, 32, WHITE);
    break;
    case 2:
      display.drawBitmap(8, 16, gps_wait2, 24, 32, WHITE);
    break;
    case 3:
      display.drawBitmap(8, 16, gps_wait3, 24, 32, WHITE);
    break;
    case 4:
      display.drawBitmap(9, 16, gps_wait4, 24, 32, WHITE);
    break;
    case 5:
      display.drawBitmap(8, 16, gps_wait5, 24, 32, WHITE);
    break;
    case 6:
      display.drawBitmap(9, 16, gps_wait4, 24, 32, WHITE);
    break;
    case 7:
      display.drawBitmap(8, 16, gps_wait3, 24, 32, WHITE);
    break;
    case 8:
      display.drawBitmap(8, 16, gps_wait2, 24, 32, WHITE);
    break;
  }
  next_image ++;
  if (next_image > 8)
    next_image = 0;
  
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(36, 20);
  display.println("Wait fix,");
  display.setCursor(36, 34);
  display.print("satellites: ");
  display.println(sat);
  display.setCursor(96, 0);
  display.println(battery_level);
  display.display();
}

void Start(String dest){
  display.clearDisplay();
  
  display.setTextSize(1);
  display.drawBitmap(52, 6, gps_wait, 24, 32, WHITE);
  display.setTextColor(SSD1306_WHITE);
  int len = dest.length();
  int x_text = (128-(len*6)) /2;
  display.setCursor(x_text, 48);
  display.print(dest);
  display.display();
}

void SDerror(){
  display.clearDisplay();
  String sd = "SD mount failed";
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  int len = sd.length();
  int x_text = (128-(len*6)) /2;
  display.setCursor(x_text, 24);
  display.print(sd);
  display.display();
}

int InvalidPoint(){
  display.clearDisplay();
  display.setTextSize(1);
  String a = "Invalid point";
  display.drawBitmap(52, 0, gps_wait1, 24, 32, WHITE);
  display.setTextColor(SSD1306_WHITE);
  int len = a.length();
  int x_text = (128-(len*6)) /2;
  display.setCursor(x_text, 36);
  display.print(a);
  a = "Save?";
  len = a.length();
  x_text = (128-(len*6)) /2;
  display.setCursor(x_text, 48);
  display.print(a);
  display.display();
  while(digitalRead(A) != LOW && digitalRead(B) != LOW);
  if(digitalRead(A) == LOW )
    return 1;
  if(digitalRead(B) == LOW )
    return 0;
  return 0;
}

void CompassAnimation(int xm, int xM, int ym, int yM, int zm, int zM){
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(1, 0);
  display.println("Compass Calibration");
  display.setCursor(6, 12);
  display.println("x");
  display.setCursor(6, 24);
  display.println("y");
  display.setCursor(6, 36);
  display.println("z");

  display.setCursor(34, 12);
  display.println(xm);
  display.setCursor(34, 24);
  display.println(ym);
  display.setCursor(34, 36);
  display.println(zm);

  display.setCursor(78, 12);
  display.println(xM);
  display.setCursor(78, 24);
  display.println(yM);
  display.setCursor(78, 36);
  display.println(zM);

  display.setCursor(1, 48);
  display.println("A: Default Calib");

  display.setCursor(1, 56);
  display.println("B: Back");
  display.display();
  
}

