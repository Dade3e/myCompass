//KEYBOARD
String keyboard(){
  int exit = true;
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
    

    Serial.print(x);
    Serial.print(" ");
    Serial.print(y);
    Serial.print(" ");
    Serial.print(keys[y][x*2]);
    Serial.print(" ");
    Serial.println(maiusc);
    
    
    display.display(); // Update screen with each newly-drawn line
    delay(10);
  }
  return testo;
}

//MENU
//Menu

void Menu(){
  int exit = true;
  while(exit){

    if(digitalRead(UP) == LOW)
      w -= 1;
    if(digitalRead(DOWN) == LOW)
      w += 1;
    if(w < 0)
      w = 3;
    if(w > 3)
      w = 0;
    if(digitalRead(A) == LOW){
      exit = false;
    }
    if(digitalRead(B) == LOW){
      exit = false;
      w = 0;
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
      display.println(menu_list[i]);
    }
    display.display();
    delay(100);
  }
  Serial.println(w);
}


//WALK
void mainWindow(String dest, int azimuth ,int dist){
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);



  display.setTextSize(1);
  display.setCursor(1, 0);
  display.println(dest);

  /*display.setTextSize(3);
  display.setCursor(10, 20);
  display.println(azimuth);*/

  display.setTextSize(1);
  display.setCursor(32, 54);
  display.print(dist);
  display.println(" m");

  //display.drawCircle(clock_center_x, clock_center_y, raggio, SSD1306_WHITE);
  draw_arrow(avanza+=4);
  
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

//set contrast
void setContrast(){
  int exit = true;
  while(exit){
    display.clearDisplay();
    display.fillRoundRect(0, 64-(cont/4), 16, (cont/4), 3, SSD1306_WHITE);
    display.drawRoundRect(0, 0, 16, display.height(), 3, SSD1306_WHITE);
    
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(36, 20);
    display.println("Set contrast");
    display.display();
    if(digitalRead(UP) == LOW)
      cont += 20;
    if(digitalRead(DOWN) == LOW)
      cont -= 20;
    if(digitalRead(A) == LOW || digitalRead(B) == LOW)
      exit = false;
    if(cont < 1)
      cont = 1;
    if(cont > 255)
      cont = 255;
    display.ssd1306_command(0x81);
    display.ssd1306_command(cont);
    Serial.println(cont);
    delay(100);
  }
}