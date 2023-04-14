#include <QMC5883LCompass.h>
#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>
#include <EEPROM.h>

static const int RXPin = 2, TXPin = 3;
static const uint32_t GPSBaud = 9600;

QMC5883LCompass compass;

TinyGPSPlus gps;

SoftwareSerial ss(RXPin, TXPin);

int calibrationData[3][2];
bool changed = false;
bool done = false;
int t = 0;
int c = 0;

bool calibrated = false;

const byte SWITCHR = 8; // pin 3 / PCINT4
const byte SWITCHL = 4; // pin 3 / PCINT4

//casa Mels 46.17725965253064, 13.11714012444914
//casa Trieste 45.63081080068116, 13.790185568089802

int s_coords = 0;

double TARGET_COORDS[10][2]{
{45.63081080, 13.79018556},  //casa Trieste
{45.62118509, 13.80372564},  //lavoro
{46.17725965, 13.11714012},  //casa Mels
{48.86546661,  2.37511937},  //casa Ele
{45.69076373, 13.78929591},   //Liverpul pub
{45.65562978, 13.78642047}
};

String TARGET_NAME[]{
  "   RONCHETO   ",
  " BIC",
  "MELS",
  " ELE",
  "   LIVERPOOL PUB   ",
  "LUCA"
};

#define CS	 7  //Pin 13 as chip selection output
#define WR	 6  //Pin 12 as read clock	output
#define DATA 5	//Pin 7 as Serial data output
 
#define CS1    digitalWrite(CS, HIGH) 
#define CS0    digitalWrite(CS, LOW)
#define WR1    digitalWrite(WR, HIGH) 
#define WR0    digitalWrite(WR, LOW)
#define DATA1  digitalWrite(DATA, HIGH) 
#define DATA0  digitalWrite(DATA, LOW)
 
 
#define sbi(x, y)  (x |= (1 << y))   
#define cbi(x, y)  (x &= ~(1 <<y ))  
#define uchar   unsigned char 
#define uint   unsigned int 
 
//定义HT1621的命令 
#define  ComMode    0x52  //4COM,1/3bias  1000    010 1001  0  
#define  RCosc      0x30  //1000 0011 0000 
#define  LCD_on     0x06  //1000     0000 0 11 0 
#define  LCD_off    0x04  
#define  Sys_en     0x02  //1000   0000 0010 
#define  CTRl_cmd   0x80  
#define  Data_cmd   0xa0  
 
//                0     1     2     3     4     5     6     7     8     9
const char num[]={0x3f, 0x09, 0x67, 0x4f, 0x59, 0x5e, 0x7e, 0x0b, 0x7f, 0x5f};
char wait[] = {0x10,0x02,0x01,0x40,0x20,0x04,0x08,0x40};
char wait2[] = {0x10,0x10,0x10,0x02,0x02,0x02,0x01,0x01,0x01,0x08,0x08,0x08,0x04,0x04,0x04,0x20,0x20,0x20};

bool fix = false;

char cara[127];

void charSet(){

  cara['0'] = 0x3f;
  cara['1'] = 0x09;
  cara['2'] = 0x67;
  cara['3'] = 0x4f;
  cara['4'] = 0x59;
  cara['5'] = 0x5e;
  cara['6'] = 0x7e;
  cara['7'] = 0x0b;
  cara['8'] = 0x7f;
  cara['9'] = 0x5f;
  
  cara['a'] = 0x6F;
  cara['b'] = 0x7C;
  cara['c'] = 0x64;
  cara['d'] = 0x6D;
  cara['e'] = 0x77;
  cara['f'] = 0x72;
  cara['g'] = 0x5F;
  cara['h'] = 0x78;
  cara['i'] = 0x20;
  cara['j'] = 0x0C;
  cara['k'] = 0x7A;
  cara['l'] = 0x30;
  cara['m'] = 0x28;
  cara['n'] = 0x68;
  cara['o'] = 0x6C;
  cara['p'] = 0x73;
  cara['q'] = 0x5B;
  cara['r'] = 0x60;
  cara['s'] = 0x5E;
  cara['t'] = 0x74;
  cara['u'] = 0x2C;
  cara['v'] = 0x2C;
  cara['w'] = 0x28;
  cara['x'] = 0x79;
  cara['y'] = 0x5D;
  cara['z'] = 0x67;

  cara['A'] = 0x7B;
  cara['B'] = 0x7F;
  cara['C'] = 0x36;
  cara['D'] = 0x6D;
  cara['E'] = 0x76;
  cara['F'] = 0x72;
  cara['G'] = 0x3E;
  cara['H'] = 0x79;
  cara['I'] = 0x30;
  cara['J'] = 0x2D;
  cara['K'] = 0x7A;
  cara['L'] = 0x34;
  cara['M'] = 0x2A;
  cara['N'] = 0x3B;
  cara['O'] = 0x3F;
  cara['P'] = 0x73;
  cara['Q'] = 0x57;
  cara['R'] = 0x33;
  cara['S'] = 0x5E;
  cara['T'] = 0x74;
  cara['U'] = 0x3D;
  cara['V'] = 0x3D;
  cara['W'] = 0x15;
  cara['X'] = 0x79;
  cara['Y'] = 0x5D;
  cara['Z'] = 0x67;

  cara['*'] = 0x53;
  cara['-'] = 0x40;
  cara['/'] = 0x61;
  cara['='] = 0x44;
  cara['\''] = 0x01;
  cara[' '] = 0x00;
  

}

void SendBit_1621(uchar sdata,uchar cnt)
{ 
	//data cnt HT1621
	uchar i; 
	for(i=0;i<cnt;i++) 
	{ 
		WR0;
		if(sdata&0x80) DATA1; 
		else DATA0;
		WR1;
		sdata<<=1; 
	} 
}
 
void SendCmd_1621(uchar command) 
{ 
	CS0; 
	SendBit_1621(0x80,4);  
	SendBit_1621(command,8); 
	CS1;                     
}
 
void Write_1621(uchar addr,uchar sdata)
{ 
	addr<<=2; 
	CS0; 
	SendBit_1621(0xa0,3);     
	SendBit_1621(addr,6);     
	SendBit_1621(sdata,8);   
	CS1; 
}
 
void HT1621_all_off(uchar num) 
{
	uchar i; 
	uchar addr=0; 
	for(i=0;i<num;i++) 
	{ 
		Write_1621(addr,0x00); 
		addr+=2; 
	} 
}
 
void HT1621_all_on(uchar num)
{
	uchar i; 
	uchar addr=0; 
	for(i=0;i<num;i++) 
	{
		Write_1621(addr,0xff); 
		addr+=2; 
	} 
}
 
void Init_1621(void)
{
	SendCmd_1621(Sys_en);
	SendCmd_1621(RCosc);    
	SendCmd_1621(ComMode);  
	SendCmd_1621(LCD_on);
}

void draw_s(String s){
  switch(s.length()){
    case 0:
      draw(0,0,0,0);
      break;
    case 1:
      draw(0,0,0,cara[s.charAt(0)]);
      break;
    case 2:
      draw(0,0,cara[s.charAt(0)],cara[s.charAt(1)]);
      break;
    case 3:
      draw(0,cara[s.charAt(0)],cara[s.charAt(1)],cara[s.charAt(2)]);
      break;
    case 4:
      draw(cara[s.charAt(0)],cara[s.charAt(1)],cara[s.charAt(2)],cara[s.charAt(3)]);
      break;
    default:
      draw(0,0,0,0);
      break;
  }
}

int draw_scroll(String s, int i){
  if(i > s.length()-4)
    i = 0;
  String view = s.substring(i, i+4);
  switch(view.length()){
    case 0:
      draw(0,0,0,0);
      break;
    case 1:
      draw(0,0,0,cara[view.charAt(0)]);
      break;
    case 2:
      draw(0,0,cara[view.charAt(0)],cara[view.charAt(1)]);
      break;
    case 3:
      draw(0,cara[view.charAt(0)],cara[view.charAt(1)],cara[view.charAt(2)]);
      break;
    case 4:
      draw(cara[view.charAt(0)],cara[view.charAt(1)],cara[view.charAt(2)],cara[view.charAt(3)]);
      break;
    default:
      draw(0,0,0,0);
      break;
  }
  return i;
}

void draw_degree(int n){
  if(n < 0)
    draw(0,0,0,0);
  else if(n >= 0 && n < 10)
    draw(0,0,num[n],0x80);
  else if(n >= 10 && n < 100)
    draw(0,num[(n / 10 % 10)],num[(n % 10)],0x80);
  else
    draw(num[(n / 100 % 10)],num[(n /10 % 10)],num[(n% 10)],0x80);
}

void draw_degree_n(int n){
  if(n < 0)
    draw(0,0,0,0);
  else if(n >= 0 && n < 10)
    draw(0,0,num[n],0x68+0x80);
  else if(n >= 10 && n < 100)
    draw(0,num[(n / 10 % 10)],num[(n % 10)],0x68+0x80);
  else
    draw(num[(n / 100 % 10)],num[(n /10 % 10)],num[(n% 10)],0x68+0x80);
}

void draw_m(int n){
  if(n < 0)
    draw(0,0,0,0);
  else if(n >= 0 && n < 10)
    draw(0,0,0,num[n]);
  else if(n >= 10 && n < 100)
    draw(0,0,num[(n / 10 % 10)],num[(n % 10)]);
  else if(n >= 100 && n < 1000)
    draw(0,num[(n / 100 % 10)],num[(n / 10 % 10)],num[(n % 10)]);
  else
    draw(num[(n / 1000 % 10)],num[(n / 100 % 10)],num[(n /10 % 10)],num[(n% 10)]);
}

void draw_dm(int n){
  if(n < 0)
    draw(0,0,0,0);
  else if(n >= 0 && n < 10)
    draw(0,0,0,num[n]);
  else if(n >= 10 && n < 100)
    draw(0,0,num[(n / 10 % 10)],num[(n % 10)]);
  else if(n >= 100 && n < 1000)
    draw(0,num[(n / 100 % 10)],num[(n / 10 % 10)],num[(n % 10)]);
  else
    draw(num[(n / 1000 % 10)] + 0x80,num[(n / 100 % 10)],num[(n /10 % 10)],num[(n% 10)]);
}

void draw_km(int n){
  if(n < 0)
    draw(0,0,0,0);
  else if(n >= 0 && n < 10)
    draw(0,0,0,num[n]);
  else if(n >= 10 && n < 100)
    draw(0,0,num[(n / 10 % 10)],num[(n % 10)]);
  else if(n >= 100 && n < 1000)
    draw(0,num[(n / 100 % 10)],num[(n / 10 % 10)],num[(n % 10)]);
  else if(n >= 100 && n < 1000)
    draw(num[(n / 1000 % 10)],num[(n / 100 % 10)],num[(n /10 % 10)],num[(n% 10)]);
  else
    draw_s("MAX");
}

void draw(uint8_t dig1, uint8_t dig2, uint8_t dig3, uint8_t dig4){

  // --2--
  //5     1
  //|--7--|
  //6     4
  // --3--
  // numero 5 = B01011110 = 0x54 = 94
  
  uint8_t raw6 = ((bitRead(dig1, 6) * 16 *8 ) + (bitRead(dig2, 6) * 16*4 ) + 
  (bitRead(dig3, 6) * 16*2) + (bitRead(dig4, 6) * 16) +
  bitRead(dig1, 7)*8 + bitRead(dig2, 7)*4 + bitRead(dig3, 7)*2 + bitRead(dig4, 7));

  uint8_t raw4 = ((bitRead(dig1, 5) * 16 *8) + (bitRead(dig2, 5) * 16*4 ) + 
  (bitRead(dig3, 5) * 16*2) + (bitRead(dig4, 5) * 16) +
  bitRead(dig1, 4)*8 + bitRead(dig2, 4)*4 + bitRead(dig3, 4)*2 + bitRead(dig4, 4));
  
  uint8_t raw2 = ((bitRead(dig1, 3) * 16 *8) + (bitRead(dig2, 3) * 16*4 ) + 
  (bitRead(dig3, 3) * 16*2) + (bitRead(dig4, 3) * 16) +
  bitRead(dig1, 2)*8 + bitRead(dig2, 2)*4 + bitRead(dig3, 2)*2 + bitRead(dig4, 2));

  uint8_t raw0 = ((bitRead(dig1, 1) * 16 *8) + (bitRead(dig2, 1) * 16*4 ) + 
  (bitRead(dig3, 1) * 16*2) + (bitRead(dig4, 1) * 16) +
  bitRead(dig1, 0) * 8 + bitRead(dig2, 0)* 4 + bitRead(dig3, 0)*2 + bitRead(dig4, 0));
  Write_1621(6,raw6);
  Write_1621(4,raw4);
  Write_1621(2,raw2);
  Write_1621(0,raw0);
}


void setup() {
  Serial.begin(9600);
  charSet();

  ss.begin(GPSBaud);
  
  pinMode (SWITCHL, INPUT);
  digitalWrite (SWITCHL, HIGH);  // internal pull-up

  pinMode (SWITCHR, INPUT);
  digitalWrite (SWITCHR, HIGH);  // internal pull-up

  pinMode(CS, OUTPUT);
  pinMode(WR, OUTPUT);
  pinMode(DATA, OUTPUT);
  CS1;
  DATA1;
  WR1;
  Init_1621();

  s_coords = readIntFromEEPROM(20);

  int f = TARGET_NAME[s_coords].length();
  if(f <= 4){
    draw_s(TARGET_NAME[s_coords]);
    delay(500);
  }
    
  else{
    int i = 2;
    for (int j =0; j<f-5; j++){
      i = draw_scroll(TARGET_NAME[s_coords], i);
      i ++;
      delay(200);
    }
  }
  //menu();
  
  //updateEEPROM();

  compass.init();
  compass.setCalibration( readIntFromEEPROM(0),readIntFromEEPROM(2), readIntFromEEPROM(4), readIntFromEEPROM(6), readIntFromEEPROM(8), readIntFromEEPROM(10));  
  //compassCalib();

}

int showCourse = 0;
uint8_t i = 0;

void loop() {
  
  while (ss.available() > 0)
    if (gps.encode(ss.read()))
  
  if(gps.location.isValid()){
    if(digitalRead(SWITCHR) == LOW){
      showCourse += 1;
      if(showCourse == 3)
        showCourse = 0;
    }
    while(digitalRead(SWITCHR) == LOW);
    if(digitalRead(SWITCHL) == LOW){
      menu();
    }
    while(digitalRead(SWITCHL) == LOW);
    if(showCourse == 0){
      double gpsCourse = TinyGPSPlus::courseTo(gps.location.lat(), gps.location.lng(), TARGET_COORDS[s_coords][0], TARGET_COORDS[s_coords][1]);
      compass.read();
      int azimut = compass.getAzimuth();
      int courseChangeNeeded = (int)(360 + gpsCourse - azimut) % 360;
      draw_degree(courseChangeNeeded);
    }
    else if(showCourse == 1){
      draw_degree_n(TinyGPSPlus::courseTo(gps.location.lat(), gps.location.lng(), TARGET_COORDS[s_coords][0], TARGET_COORDS[s_coords][1]));
    }
    else{
      double dist = TinyGPSPlus::distanceBetween(gps.location.lat(), gps.location.lng(), TARGET_COORDS[s_coords][0], TARGET_COORDS[s_coords][1]);
      if(dist > 9999)
        draw_km(dist/1000);
      else if(dist > 999)
        draw_dm(dist);
      else
        draw_m(dist);
    }
  }else{
    Serial.println("Attesa gps");
    if(digitalRead(SWITCHL) == LOW){
      menu();
    }
    while(digitalRead(SWITCHR) == LOW && digitalRead(SWITCHL) == LOW){delay(10);}
    draw(wait2[i],0x3E,0x73,0x5E);
    i++;
    if(i == 18)
      i = 0;
  }
    
}

void menu(){
  draw_s("MENU");
  delay(650);
  draw_s("");
  delay(50);
  uint8_t select = 1;
  int i = 2;
  int wait = 0;
  int text_vel = 4;
  while(select != 99){
    
    if(digitalRead(SWITCHL) == LOW){
      select += 1;
      wait = 0;
      i = 2;      
    }
      
    while(digitalRead(SWITCHL) == LOW);

    switch(select){
      case(1):
        i = draw_scroll("   SELECT POINT   ", i);
        wait ++;
        if(wait == text_vel){
          wait = 0;
          i++;          
        }
        if(digitalRead(SWITCHR) == LOW)
          select = 10;
        while(digitalRead(SWITCHR) == LOW);
      break;
      case(2):
      case(4):
        i = 2;
        wait = 0;
        select +=1;
      break;
      case(3):
        i = draw_scroll("   CALIB   ", i);
        wait ++;
        if(wait == text_vel){
          wait = 0;
          i++;          
        }
        if(digitalRead(SWITCHR) == LOW)
          select = 11;
        while(digitalRead(SWITCHR) == LOW);
      break;
      case(5):
        draw_s("EXIT");
        if(digitalRead(SWITCHR) == LOW)
          select = 99;
        while(digitalRead(SWITCHR) == LOW);
      break;
      case(10):
        select_coords();
        i = 2;
        wait = 0;
        select = 1;
      break;
      case(11):
        compassCalib();
        i = 2;
        wait = 0;
        select = 3;
      break;
      default:
        select = 1;
        break;
      
    }
    delay(50);
  }

}

void select_coords(){
  bool exit = false;
  uint8_t select = s_coords;
  int i = 2;
  int wait = 0;
  while(digitalRead(SWITCHR) == HIGH){
    if(digitalRead(SWITCHL) == LOW){
      select += 1;
      i = 2;
      wait = 0;
    }
      
    while(digitalRead(SWITCHL) == LOW);
    if(select >= (sizeof(TARGET_NAME) / sizeof(TARGET_NAME[0])))
      select = 0;
    i = draw_scroll(TARGET_NAME[select], i);
    wait ++;
    if(wait == 4){
      wait = 0;
      i++;          
    }
    delay(50);
  }
  
  s_coords = select;
  writeIntIntoEEPROM(20, s_coords);
  draw_s("OK ");
  delay(1000);
}

void updateEEPROM(){
  writeIntIntoEEPROM(0, calibrationData[0][0]);
  writeIntIntoEEPROM(2, calibrationData[0][1]);
  writeIntIntoEEPROM(4, calibrationData[1][0]);
  writeIntIntoEEPROM(6, calibrationData[1][1]);
  writeIntIntoEEPROM(8, calibrationData[2][0]);
  writeIntIntoEEPROM(10, calibrationData[2][1]);
}

void writeIntIntoEEPROM(int address, int number)
{ 
  EEPROM.write(address, number >> 8);
  EEPROM.write(address + 1, number & 0xFF);
}

int readIntFromEEPROM(int address)
{
  return (EEPROM.read(address) << 8) + EEPROM.read(address + 1);
}

void compassCalib(){
  draw_s("CLB");
  while(calibrated == false){
    // If not calibrated
    int x, y, z;

    // Read compass values
    compass.read();

    // Return XYZ readings
    x = compass.getX();
    y = compass.getY();
    z = compass.getZ();

    changed = false;

    if(x < calibrationData[0][0]) {
      calibrationData[0][0] = x;
      changed = true;
    }
    if(x > calibrationData[0][1]) {
      calibrationData[0][1] = x;
      changed = true;
    }

    if(y < calibrationData[1][0]) {
      calibrationData[1][0] = y;
      changed = true;
    }
    if(y > calibrationData[1][1]) {
      calibrationData[1][1] = y;
      changed = true;
    }

    if(z < calibrationData[2][0]) {
      calibrationData[2][0] = z;
      changed = true;
    }
    if(z > calibrationData[2][1]) {
      calibrationData[2][1] = z;
      changed = true;
    }

    if (changed && !done) {
      Serial.println("CALIBRATING... Keep moving your sensor around.");
      c = millis();
      draw(wait[i],0x36,0x34,0x7F);
      i++;
      if(i == 8)
        i = 0;
    }
    t = millis();


    if ( (t - c > 5000) && !done) {
      done = true;
      Serial.println("DONE.");
      Serial.println();

      Serial.print("compass.setCalibration(");
      Serial.print(calibrationData[0][0]);
      Serial.print(", ");
      Serial.print(calibrationData[0][1]);
      Serial.print(", ");
      Serial.print(calibrationData[1][0]);
      Serial.print(", ");
      Serial.print(calibrationData[1][1]);
      Serial.print(", ");
      Serial.print(calibrationData[2][0]);
      Serial.print(", ");
      Serial.print(calibrationData[2][1]);
      Serial.println(");");

      compass.setCalibration( calibrationData[0][0], calibrationData[0][1], calibrationData[1][0],
                              calibrationData[1][1], calibrationData[2][0], calibrationData[2][1]);
      calibrated = true;
    }
  }
  calibrated = false;
  updateEEPROM();

  draw_s("OK ");

  delay(1000);
}