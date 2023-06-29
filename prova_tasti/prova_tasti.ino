/*
  AnalogReadSerial

  Reads an analog input on pin 0, prints the result to the Serial Monitor.
  Graphical representation is available using Serial Plotter (Tools > Serial Plotter menu).
  Attach the center pin of a potentiometer to pin A0, and the outside pins to +5V and ground.

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/AnalogReadSerial
*/
const byte UP = 2;
const byte DOWN = 4;
const byte LEFT = 15;
const byte RIGHT = 25;
const byte A = 26;
const byte B = 27;

const byte battery = 33;


// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(115200);
  pinMode(UP, INPUT_PULLUP);
  pinMode(DOWN, INPUT_PULLUP);
  pinMode(LEFT, INPUT_PULLUP);
  pinMode(RIGHT, INPUT_PULLUP);
  pinMode(A, INPUT_PULLUP);
  pinMode(B, INPUT_PULLUP);
  pinMode(battery, INPUT);
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 0:
  // print out the value you read:
  Serial.println("Up  Down  Left  right  A  B  Battery");
  Serial.print(" ");
  Serial.print(digitalRead(UP));
  Serial.print("   ");
  Serial.print(digitalRead(DOWN));
  Serial.print("     ");
  Serial.print(digitalRead(LEFT));
  Serial.print("     ");
  Serial.print(digitalRead(RIGHT));
  Serial.print("     ");
  Serial.print(digitalRead(A));
  Serial.print("  ");
  Serial.print(digitalRead(B));
  Serial.print("    ");
  Serial.print(analogRead(battery));
  Serial.println();
  Serial.println();
  delay(100);  // delay in between reads for stability
}
