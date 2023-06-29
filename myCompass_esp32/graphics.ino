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