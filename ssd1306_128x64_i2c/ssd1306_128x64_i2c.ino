#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

const byte UP = 2;
const byte DOWN = 4;
const byte LEFT = 15;
const byte RIGHT = 25;
const byte A = 26;
const byte B = 27;

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


int x = 5;
int y = 3;
String testo = "";
int maiusc = 0;
String keys[] = { "0 1 2 3 4 5 6 7 8 9 <",
                    "q w e r t y u i o p _",
                    ". a s d f g h j k l -",
                    "^ z x c v   b n m END"};
String keysM[] = { "0 1 2 3 4 5 6 7 8 9 <",
                  "Q W E R T Y U I O P _",
                  ". A S D F G H J K L -",
                  "^ Z X C V   B N M END"};



void setup() {
  Serial.begin(9600);

  pinMode(UP, INPUT_PULLUP);
  pinMode(DOWN, INPUT_PULLUP);
  pinMode(LEFT, INPUT_PULLUP);
  pinMode(RIGHT, INPUT_PULLUP);
  pinMode(A, INPUT_PULLUP);
  pinMode(B, INPUT_PULLUP);

  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.setRotation(2);
}

void loop() {
  
  Serial.println(keyboard());
  mainWindow("Destinazione", 270 , 1980);
  while(digitalRead(A) == LOW);
  while(digitalRead(B) == LOW);
  delay(50);
}


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

    display.setTextSize(1);
    display.setCursor(1, 6);
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
    
    /*
    Serial.print(x);
    Serial.print(" ");
    Serial.print(y);
    Serial.print(" ");
    Serial.print(keys[y][x*2]);
    Serial.print(" ");
    Serial.println(maiusc);
    */
    
    display.display();
    delay(100);
  }
  return testo;
}


const float pi = 3.14159267 ;
int raggio = 30;
int raggioBase = 30;
int avanza = 0;
int clock_center_y = 32;
int clock_center_x = 96;
int vert = 130;


void mainWindow(String dest, int azimuth ,int dist){
  while(digitalRead(B) == HIGH){
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);



    display.setTextSize(1);
    display.setCursor(1, 0);
    display.println(dest);

    display.setTextSize(3);
    display.setCursor(10, 20);
    display.println(avanza);

    display.setTextSize(1);
    display.setCursor(1, 54);
    display.print(dist);
    display.println(" m");

    //display.drawCircle(clock_center_x, clock_center_y, raggio, SSD1306_WHITE);
    draw_arrow(avanza+=4);
    if(avanza >= 360)
      avanza = 0;
    
    //display.setTextSize(1);
    //display.setCursor(clock_center_x-(raggio/2),clock_center_y-3);
    //display.println(azimuth);
    display.display();
    delay(50);
  }
  
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

  display.fillTriangle(x1,y1,x2,y2,clock_center_x,clock_center_y, SSD1306_INVERSE);
  display.fillTriangle(x,y,x1,y1,x2,y2, SSD1306_INVERSE);
  display.display();
}