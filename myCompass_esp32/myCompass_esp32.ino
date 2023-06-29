#include <QMC5883LCompass.h>
#include <TinyGPSPlus.h>
#include <EEPROM.h>
#include "FS.h"
#include "SD.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

static const uint32_t GPSBaud = 9600;

QMC5883LCompass compass;

TinyGPSPlus gps;

//SoftwareSerial ss(RXPin, TXPin);

int calibrationData[3][2];
bool changed = false;
bool done = false;
int t = 0;
int c = 0;

bool calibrated = false;

const byte UP = 2;
const byte DOWN = 4;
const byte LEFT = 15;
const byte RIGHT = 25;
const byte A = 26;
const byte B = 27;
const byte battery = 33;

//casa Mels 46.17725965253064, 13.11714012444914
//casa Trieste 45.63081080068116, 13.790185568089802

int s_coords = 0;

double TARGET_COORDS[] = {45.63081080, 13.79018556};

String TARGET_NAME = "";

//keyboard
int x = 5;
int y = 3;
int w = 0;
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

String menu_list[] = {"Walk", "Select point", "Save Point", "Calib"};

//Arrow
const float pi = 3.14159267 ;
int raggio = 22;
int raggioBase = 22;
int avanza = 0;
int clock_center_y = 32;
int clock_center_x = 64;
int vert = 130;


void setup() {
  Serial.begin(115200);
  charSet();

  Serial2.begin(GPSBaud);
  
  pinMode(UP, INPUT_PULLUP);
  pinMode(DOWN, INPUT_PULLUP);
  pinMode(LEFT, INPUT_PULLUP);
  pinMode(RIGHT, INPUT_PULLUP);
  pinMode(A, INPUT_PULLUP);
  pinMode(B, INPUT_PULLUP);
  pinMode(battery, INPUT);

  if(!SD.begin(5)){
    Serial.println("Card Mount Failed");
    return;
  }

  String index = readIndex(SD, "/index.txt");
  TARGET_NAME = split(index, ',', 1);

  s_coords = split(index, ',', 0);


  draw(TARGET_NAME[s_coords]);
  delay(500);
  //menu();

  compass.init();
  String calib = readFileString(SD, "/calib.txt");
  compass.setCalibration(split(calib, '\n', 0).toInt(),split(calib, '\n', 1).toInt(),
                         split(calib, '\n', 2).toInt(), split(calib, '\n', 3).toInt(), 
                         split(calib, '\n', 4).toInt(), split(calib, '\n', 5).toInt());  
  //compassCalib();

}

int showCourse = 0;
uint8_t i = 0;

void loop() {
  
  while (Serial2.available() > 0)
    if (gps.encode(Serial2.read()))
  
  if(gps.location.isValid()){
    if(digitalRead(RIGHT) == LOW){
      showCourse += 1;
      if(showCourse == 3)
        showCourse = 0;
    }
    while(digitalRead(RIGHT) == LOW);
    if(digitalRead(LEFT) == LOW){
      menu();
    }
    while(digitalRead(LEFT) == LOW);
    if(showCourse == 0){
      double gpsCourse = TinyGPSPlus::courseTo(gps.location.lat(), gps.location.lng(), TARGET_COORDS[0], TARGET_COORDS[1]);
      compass.read();
      int azimut = compass.getAzimuth();
      int courseChangeNeeded = (int)(360 + gpsCourse - azimut) % 360;
      draw_degree(courseChangeNeeded);
    }
    else if(showCourse == 1){
      draw_degree_n(TinyGPSPlus::courseTo(gps.location.lat(), gps.location.lng(), TARGET_COORDS[0], TARGET_COORDS[1]));
    }
    else{
      double dist = TinyGPSPlus::distanceBetween(gps.location.lat(), gps.location.lng(), TARGET_COORDS[0], TARGET_COORDS[1]);
      if(dist > 9999)
        draw_km(dist/1000);
      else if(dist > 999)
        draw_dm(dist);
      else
        draw_m(dist);
    }
  }else{
    Serial.println("Attesa gps");
    if(digitalRead(LEFT) == LOW){
      menu();
    }
    while(digitalRead(RIGHT) == LOW && digitalRead(LEFT) == LOW){delay(10);}
    draw(wait2[i],0x3E,0x73,0x5E);
    i++;
    if(i == 18)
      i = 0;
  }
    
}


