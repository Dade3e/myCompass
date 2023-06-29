//#include <SoftwareSerial.h>

const byte rxPin = 2;
const byte txPin = 3;

// Set up a new SoftwareSerial object
//SoftwareSerial mySerial (rxPin, txPin);

void setup() {
  //init seriale hardware
  Serial.begin(115200);
  //init seriale software
  Serial2.begin(9600);
  delay(3000);
  Serial2.println("$PMTK220,10000*2F");
  //Serial2.println("$PSRF103,00,01,10,01*25");
}

void loop() {
  //controllo se nella seriale software ci sono dati
  if (Serial2.available()) {
    //se sono presenti dati leggili ed inviali
    //alla seriale hardware
    Serial.write(Serial2.read());
  }
}