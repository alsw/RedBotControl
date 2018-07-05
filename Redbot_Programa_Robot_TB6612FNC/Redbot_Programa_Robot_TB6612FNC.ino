int MotorD[2] = {3, 6};
int MotorI[2] = {9, 11};

unsigned int estadoTrama = 0;

int valX = 0;
int valY = 0;
int motorL = 0;
int motorR = 0;

char bufferNum[32];
int pBuffer = 0;
char coord = 0;

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 2; i++) {
    pinMode(MotorD, OUTPUT);
    pinMode(MotorI, OUTPUT);
  }
}

void loop() {
  char caracter;
  int valor;

  while (Serial.available()) {
    caracter = Serial.read();
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

  Mover(MotorD, motorL);
  Mover(MotorI, motorR);
}

void Mover(int Motor[], int Valor) {
  if (Valor > 0) {
    analogWrite(Motor[0], Valor);
    analogWrite(Motor[1], 0);
  }
  else {
    Valor = -Valor;
    analogWrite(Motor[0], 0);
    analogWrite(Motor[1], Valor);
  }
}
