char Cordenada = ' ';
int Estado = 0;

int PunteroAreglo = 0;
char AregloNumero[10];

int ValorX = 0;
int ValorY = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  if (Serial.available()) {
    char Letra = Serial.read();
    ProcesarSerial(Letra);
  }

  Serial.print("ValorX: ");
  Serial.print(ValorX);
  Serial.print(" ValorY: ");
  Serial.println(ValorY);
  delay(100);
}

void ProcesarSerial(char Letra) {
  switch (Estado) {
    case 0:
      if (Letra == 'x' || Letra == 'X') {
        Cordenada = 'X';
        Estado = 1;
        PunteroAreglo = 0;
        AregloNumero[0] = 0;
      } else if (Letra == 'y' || Letra == 'Y') {
        Cordenada = 'Y';
        Estado = 1;
        PunteroAreglo = 0;
        AregloNumero[0] = 0;
      }
      break;
    case 1:
      if (Letra >= '0' && Letra <= '9') {
        AregloNumero[PunteroAreglo] = Letra;
        PunteroAreglo++;
      } else if (Letra == '-' && PunteroAreglo == 0) {
        AregloNumero[0] = '-';
        PunteroAreglo++;
      } else {
        AregloNumero[PunteroAreglo] = 0;
        int ValorTemperal = atoi(AregloNumero);
        if (Cordenada == 'X') {
          ValorX = ValorTemperal;
        }
        else if (Cordenada = 'Y') {
          ValorY = ValorTemperal;
        }
        Estado = 0;
      }
      break;
  }
}

