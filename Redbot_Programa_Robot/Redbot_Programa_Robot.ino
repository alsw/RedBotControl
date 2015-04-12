#include <RedBot.h>

RedBotMotor motor;
RedBotSoftwareSerial redbotSerial;

unsigned int estadoTrama = 0;

int valX = 0;
int valY = 0;
int motorL = 0;
int motorR = 0;

char bufferNum[32];
int pBuffer = 0;
char coord = 0;

void setup() {
  //Serial.begin(9600);//This will open the serial port to send values out to calibrate sensors
  redbotSerial.begin(9600);
  redbotSerial.println("Hello world!");//This should print once to make sure you're getting serial communication
}

void loop() {
  char caracter;
  int valor;

  while (redbotSerial.available()) {
    caracter = redbotSerial.read();
    switch (estadoTrama) {
      case 0:
        if (caracter == 'x' || caracter == 'X') {
          coord = 'X';
          estadoTrama = 1;
          bufferNum[0] = 0;
          pBuffer = 0;
        }
        if (caracter == 'y' || caracter == 'Y') {
          coord = 'Y';
          estadoTrama = 1;
          bufferNum[0] = 0;
          pBuffer = 0;
        }
        break;
      case 1:
        if (caracter == '-' && pBuffer == 0) {
          bufferNum[0] = '-';
          pBuffer = 1;
        }
        else if (caracter >= '0' && caracter <= '9') {
          bufferNum[pBuffer] = caracter;
          pBuffer++;
        }
        else {
          bufferNum[pBuffer] = 0;
          valor = atoi(bufferNum);
          if (valor > 100) valor = 100;
          if (valor < -100) valor = -100;

          if (coord == 'X')
            valX = valor;
          if (coord == 'Y')
            valY = valor;

          redbotSerial.print("X=");
          redbotSerial.print(valX);
          redbotSerial.print(" Y=");
          redbotSerial.println(valY);
          estadoTrama = 0;
        }
        break;
    }
  }

  motorL = map(valY, -100, 100, -255, 255);
  motorL -= map(valX, -100, 100, -255, 255);
  motorR = map(valY, -100, 100, -255, 255);
  motorR += map(valX, -100, 100, -255, 255);

  motorL = constrain(motorL, -255, 255);
  motorR = constrain(motorR, -255, 255);

  motor.leftDrive(motorL);
  motor.rightDrive(motorR);
}
