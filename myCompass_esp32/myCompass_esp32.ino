#include <QMC5883LCompass.h>
#include <TinyGPSPlus.h>
#include <EEPROM.h>
#include "FS.h"
#include "SD.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "./icons.h"

static const uint32_t GPSBaud = 9600;

QMC5883LCompass compass;

TinyGPSPlus gps;

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

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

double s_coords = 0;

double TARGET_COORDS[] = {45.63081080, 13.79018556};

String TARGET_NAME = "";

//keyboard/menu/brightness/wait fix
int w = 0;
int brigh = 128;
int next_image = 0;
int selected = 0;
int maiusc = 0;
double battery_level = 0;
String keys[] = { "0 1 2 3 4 5 6 7 8 9 <",
                    "q w e r t y u i o p _",
                    ". a s d f g h j k l -",
                    "^ z x c v   b n m END"};
String keysM[] = { "0 1 2 3 4 5 6 7 8 9 <",
                  "Q W E R T Y U I O P _",
                  ". A S D F G H J K L -",
                  "^ Z X C V   B N M END"};\
String num[] = {  "1 2 3 <",
                  "4 5 6 .",
                  "7 8 9 .",
                  "0 . END"};

String menu_list[] = {"Walk", "Select point", "Save Point", "Create Point", "Compass calib", "Set brightness"};

//Arrow
const float pi = 3.14159267 ;
int raggio = 22;
int raggioBase = 22;
int clock_center_y = 32;
int clock_center_x = 96;
int vert = 130;


void setup() {
  Serial.begin(115200);

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

  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.setRotation(2);
  display.ssd1306_command(0x81);
  display.ssd1306_command(brigh);

  String index = readFileString(SD, "/index.txt");
  TARGET_NAME = split(index, ',', 1);
  Serial.println(index);
  TARGET_COORDS[0] = split(index, ',', 2).toDouble();
  TARGET_COORDS[1] = split(index, ',', 3).toDouble();

  s_coords = split(index, ',', 0).toDouble();
  brigh  = split(index, ',', 4).toInt();


  Start(TARGET_NAME);
  delay(2000);

  compass.init();
  String calib = readFileString(SD, "/calib.txt");
  compass.setCalibration(split(calib, '\n', 0).toInt(),split(calib, '\n', 1).toInt(),
                         split(calib, '\n', 2).toInt(), split(calib, '\n', 3).toInt(), 
                         split(calib, '\n', 4).toInt(), split(calib, '\n', 5).toInt());  
  //compassCalib();

}

int stato = 0;
double now_lat = 0;
double now_lng = 0;

void loop() {
  if(digitalRead(A) == LOW){
    stato = Menu();
  }
  while(digitalRead(A) == LOW);
  
  //WALK
  if(stato == 0){
    while (Serial2.available() > 0)
      if (gps.encode(Serial2.read()))
    
    if(gps.location.isValid()){
      battery_level = (analogRead(battery) * 0.0007) + 1.418;
      double gpsCourse = TinyGPSPlus::courseTo(gps.location.lat(), gps.location.lng(), TARGET_COORDS[0], TARGET_COORDS[1]);
      compass.read();
      int azimut = compass.getAzimuth();
      int courseChangeNeeded = (int)(360 + gpsCourse - azimut) % 360;
      double dist = TinyGPSPlus::distanceBetween(gps.location.lat(), gps.location.lng(), TARGET_COORDS[0], TARGET_COORDS[1]);
      Walk(TARGET_NAME, courseChangeNeeded, dist, gps.satellites.value());
      now_lat = gps.location.lat();
      now_lng = gps.location.lng();
    }
    else{
      //Serial.print("Attesa gps, satelliti: ");
      //Serial.println(gps.satellites.value());
      waitFix(gps.satellites.value());
    }
  }

  //SELECT POINT  
  if(stato == 1){
    SelectPoint();
    stato = Menu();
  }

  //SAVE POINT  
  if(stato == 2){
    SavePoint();
    stato = Menu();
  }

  //SET POINT  
  if(stato == 3){
    SetPoint();
    stato = Menu();
  }

  //COMPASS CALIBRATION
  if(stato == 4){
    compassCalib();
    stato = Menu();    
  }

  //SET BRIGHTNESS
  if(stato == 5){
    setbrightness();
    stato = Menu();    
  }
}

void SavePoint(){
  while(digitalRead(A) == LOW);
  while(digitalRead(B) == LOW);
  if (now_lat == 0 || now_lng== 0){
    if(InvalidPoint()){
      while(digitalRead(A) == LOW);
      while(digitalRead(B) == LOW);
      String nome = keyboard("");
      if(nome != ""){
        String abcd = nome + "," + now_lat + "," + now_lng+"\n";
        Serial.println(abcd.c_str());
        appendFile(SD,"/coords.txt", abcd.c_str());    
      }
    }
  }
  while(digitalRead(A) == LOW);
  while(digitalRead(B) == LOW);
}

void SetPoint(){
  while(digitalRead(A) == LOW);
  while(digitalRead(B) == LOW);

  String nome = keyboard("Name: ");
  if(nome == "")
    return;
  String lat_tmp = Numpad("Lat: ");
  if(lat_tmp == "")
    return;
  String lng_tmp = Numpad("Lng: ");
  if(lat_tmp == "")
    return;
  
  String abcd = nome + "," + lat_tmp + "," + lng_tmp+"\n";
  Serial.println(abcd.c_str());
  appendFile(SD,"/coords.txt", abcd.c_str());
  while(digitalRead(A) == LOW);
  while(digitalRead(B) == LOW);
}

