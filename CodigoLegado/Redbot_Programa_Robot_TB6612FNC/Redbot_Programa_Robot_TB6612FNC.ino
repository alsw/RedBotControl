int MotorD[2] = {13, 12};
int MotorI[2] = {10, 9};
int pwmD = 6;
int pwmI = 5;
int StSy = 11;

unsigned int estadoTrama = 0;

int valX = 0;
int valY = 0;
int VelocidadD = 0;
int VelocidadI = 0;

char bufferNum[32];
int pBuffer = 0;
char coord = 0;

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 2; i++) {
    pinMode(MotorD, OUTPUT);
    pinMode(MotorI, OUTPUT);
  }

  pinMode(pwmD, OUTPUT);
  pinMode(pwmI, OUTPUT);

  pinMode(StSy, OUTPUT);
  digitalWrite(StSy, 1);
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

  VelocidadD = map(valY, -100, 100, -255, 255);
  VelocidadD -= map(valX, -100, 100, -255, 255);
  VelocidadI = map(valY, -100, 100, -255, 255);
  VelocidadI += map(valX, -100, 100, -255, 255);

  VelocidadD = constrain(VelocidadD, -255, 255);
  VelocidadI = constrain(VelocidadI, -255, 255);

  Mover(MotorD, VelocidadD, pwmD);
  Mover(MotorI, VelocidadI, pwmI);
}

void Mover(int Motor[], int Valor, int pwm) {
 if (Valor > 0) {
    digitalWrite(Motor[0], 1);
    digitalWrite(Motor[1], 0);
  }
  else {
    Valor = -Valor;
    digitalWrite(Motor[0], 0);
    digitalWrite(Motor[1], 1);
  }
  analogWrite(pwm, Valor);
}
