#include <SoftwareSerial.h>

#define GPS_TARGET_LAT      45.63081080
#define GPS_TARGET_LNG      13.79018556

#define CS	 4  //Pin 13 as chip selection output
#define WR	 2  //Pin 12 as read clock	output
#define DATA 1	//Pin 7 as Serial data output
 
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

char cara[127];
//pin 2 RX e pin 3 TX
SoftwareSerial mySerial(0, 3);
String NMEA;
int posizione_delimitatore = 0;
int index = 0;
int campo = 0;

double my_lat;
double my_lng;

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

void draw_n(int n){
  if(n < 1)
    draw(0,0,0,0);
  if(n >= 0 && n < 10)
    draw(0,0,0,num[n]);
  if(n >= 10 && n < 100)
    draw(0,0,num[(n / 10 % 10)],num[(n % 10)]);
  if(n >= 100 && n < 1000)
    draw(0,num[(n / 100 % 10)],num[(n /10 % 10)],num[(n% 10)]);
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
  //init seriale software
  mySerial.begin(9600);

  pinMode(CS, OUTPUT); // 
  pinMode(WR, OUTPUT); // 
  pinMode(DATA, OUTPUT); //
  CS1;
  DATA1;
  WR1;
  Init_1621();

}

unsigned int Start = millis();

void loop() {
  String nmea_lat = "";
  String nmea_lng = "";
  if (mySerial.available()) {
    //leggo la prima riga
    char c = mySerial.read();
    
    if (c != '\n') {
      NMEA += c;
    }
    else {
      posizione_delimitatore = 0;
      index = 0;
      //questa variabile viene usata per contare tutti i campi trovati
      campo = 0;
      //printo solo la stringa che mi interessa
      if (NMEA.substring(0, 6) == "$GPGGA")
      {
        //Serial.println(NMEA);

        while (posizione_delimitatore != -1)
        {
          posizione_delimitatore = NMEA.indexOf(",", index);

          //in funzione del campo invio al serial monitor
          //solo i dati che mi interessano
          switch (campo)
          {
            case 2:
              my_lat = NMEA.substring(index, posizione_delimitatore).toDouble();
              break;
            case 4:
              my_lng = NMEA.substring(index, posizione_delimitatore).toDouble();
              break;
          }

          index = posizione_delimitatore;
          index++;
          //incrementa il campo trovato
          campo++;
        }
      }
      draw_n(courseTo(my_lat, my_lng, GPS_TARGET_LAT, GPS_TARGET_LNG));
      NMEA = "";
            
    }  
  }
  
  
}


double distanceBetween( long double lat1, long double long1,  
                    long  double lat2,long  double long2) 
{ 
  double delta = radians(long1-long2);
  double sdlong = sin(delta);
  double cdlong = cos(delta);
  lat1 = radians(lat1);
  lat2 = radians(lat2);
  double slat1 = sin(lat1);
  double clat1 = cos(lat1);
  double slat2 = sin(lat2);
  double clat2 = cos(lat2);
  delta = (clat1 * slat2) - (slat1 * clat2 * cdlong);
  delta = sq(delta);
  delta += sq(clat2 * sdlong);
  delta = sqrt(delta);
  double denom = (slat1 * slat2) + (clat1 * clat2 * cdlong);
  delta = atan2(delta, denom);
  return delta * 6372795;
}

double courseTo(double lat1, double long1, double lat2, double long2)
{
  // returns course in degrees (North=0, West=270) from position 1 to position 2,
  // both specified as signed decimal-degrees latitude and longitude.
  // Because Earth is no exact sphere, calculated course may be off by a tiny fraction.
  // Courtesy of Maarten Lamers
  double dlon = radians(long2-long1);
  lat1 = radians(lat1);
  lat2 = radians(lat2);
  double a1 = sin(dlon) * cos(lat2);
  double a2 = sin(lat1) * cos(lat2) * cos(dlon);
  a2 = cos(lat1) * sin(lat2) - a2;
  a2 = atan2(a1, a2);
  if (a2 < 0.0)
  {
    a2 += TWO_PI;
  }
  return degrees(a2);
}


double parseDegrees(String term)
{
  if(term == "")
    return -1;
  float tmp = term.toFloat();
  long tmp2 = tmp * 10000L;
  int dd = tmp2/1000000;
  return ((tmp2%1000000)/10000 / 60)+dd;
  
}