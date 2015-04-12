#include <SoftwareSerial.h>

SoftwareSerial puertoBT(8, 9);

void setup() {
  Serial.begin(9600);
  puertoBT.begin(9600);
}

void loop() {
  int valX, valY;

  while (puertoBT.available()) puertoBT.read();

  valX = map(analogRead(0), 0, 1023, -100, 100);
  valY = map(analogRead(1), 0, 1023, -100, 100);

  //Serial.print("X=");
  //Serial.print(valX);
  //Serial.print(" Y=");
  //Serial.println(valY);
  puertoBT.print("X");
  puertoBT.println(valX);
  delay(50);
  puertoBT.print("Y");
  puertoBT.println(valY);
  delay(50);
}
