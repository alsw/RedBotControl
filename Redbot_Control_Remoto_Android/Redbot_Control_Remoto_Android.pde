import java.util.*;
import ketai.ui.*;
import ketai.net.bluetooth.*;
import ketai.sensors.*;

KetaiBluetooth Kbt;
KetaiSensor Ksen;
ArrayList listaDisp;
String nombreDisp;
float controlX, controlY; //Valores de control, de -1.0 a +1.0
int tAnt;                 //Tiempo de la ultima actualizacion en ms
float accelX, accelY;     //Valores del acelerometro

float aTactX = 1.0;  //Dimensiones del area de control tactil
float aTactY = 0.9;  //Deja el 10% para los botones al fondo
int pantX, pantY;    //Dimensiones usables de la pantalla

boolean accelActivo = false;

void setup() {
  //Se asegura de que no se pueda girar la pantalla y la abarca a
  //totalidad
  orientation(PORTRAIT);
  size(displayWidth, displayHeight);

  //Calcula las dimensiones usables de la pantalla
  pantX = int(width*aTactX);       
  pantY = int(height*aTactY);

  //Inicializa las clases de bluetooth y sensores
  Kbt = new KetaiBluetooth(this);
  Ksen = new KetaiSensor(this);
  Ksen.start();

  //Obtiene la lista de dispositivos bluetooth emparejados
  listaDisp = Kbt.getPairedDeviceNames();

  //Imprime la lista en terminal
  Iterator itr = listaDisp.iterator();
  print("Dispositivos emparejados:");
  while (itr.hasNext()) {
    print(itr.next());
  }
}

void draw() {
  //Actualiza el tiempo actual
  int tAct = millis();
  String Buffer;

  //Efectu la logica de control segun el modo de operacion
  if (accelActivo)
    controlAcelerometro();
  else
    controlTactil();

  //Realiza el dibujado de la pantalla
  dibujarPantalla();

  //Si existe un dispositivo seleccionado, realiza la transmision
  //de las coordenadas X y Y del cursor en intervalos periodicos
  //(20 veces por segundo)
  if (tAct - tAnt > 50 && nombreDisp != null) {
    Buffer = String.format("X%d\n", int(controlX*100));
    Kbt.writeToDeviceName(nombreDisp, Buffer.getBytes());
    Buffer = String.format("Y%d\n", int(controlY*100));
    Kbt.writeToDeviceName(nombreDisp, Buffer.getBytes());
    tAnt = tAct;
  }
}

void onKetaiListSelection(KetaiList klist) {
  //Obtiene la cadena recien seleccionada
  nombreDisp = klist.getSelection();

  //Imprime el nombre de dispositivo en terminal
  print("Dispositivo seleccionado:");
  print(nombreDisp);

  //Efectua la conexion bluetooth al dispositivo seleccionado
  Kbt.connectToDeviceByName(nombreDisp);

  //Desaloja la lista, ya no es necesaria
  klist = null;
}

void onAccelerometerEvent(float x, float y, float z, long time, int accuracy)
{
  accelX = -x;
  accelY = -y;
}

void controlAcelerometro() {
  controlX = constrain(accelX/9.8*1.25, -1.0, 1.0);
  controlY = constrain(accelY/9.8*1.25, -1.0, 1.0);
}

void controlTactil() {
  //Determina si el usuario a tocado la pantalla
  if (mousePressed && mouseY < pantY) {
    //De ser asi, normaliza las coordenadas del cursor (mas bien
    //dedo), de forma que abarquen el rango que va de -1.0 a +1.0
    controlX = float(mouseX)/(pantX-1)*2 - 1.0;
    controlY = 1.0 - float(mouseY)/(pantY-1)*2;
  }
  else {
    //Si el usuario no toca la pantalla, las coordenadas se hacen 0
    controlX = 0;
    controlY = 0;
  }
}

void mousePressed() {
  int alturaBarra = height - pantY;
  int anchoBoton = pantX / 2;

  //Detecta clicks sobre el boton del acelerometro
  if (mouseX < anchoBoton && mouseY >= pantY) {
    //Conmuta el estado del acelerometro
    accelActivo = !accelActivo;
  }

  //Detecta clicks sobre el boton de conexion
  if (mouseX >= anchoBoton && mouseY >= pantY) {
    //Crea una nueva lista de seleccion en pantalla para que el
    //usuario elija un dispositivo bluetooth
    KetaiList klist = new KetaiList(this , listaDisp);
  }
}

void dibujarPantalla() {
  int tamX, tamY;    //Dimensiones del rectangulo de la mira
  int posX, posY;    //Posicion en pixeles del cursor
  int alturaBarra = height - pantY;  //Altura de la barra inferior
  int anchoBoton = pantX / 2;        //Anchura de los botones

  //Calcula las dimensiones del rectangulo de la mira para que
  //sean un quinto de la resolucion horizontal
  tamX = pantX/5;
  tamY = tamX;

  //Calcula la posicion en pixeles del cursor
  posX = int((controlX+1.0)*pantX/2);
  posY = int((1.0-controlY)*pantY/2);

  //Limpia el fondo con color negro
  background(0);

  //Dibuja la mira en rojo
  stroke(255, 0, 0);
  line(posX, 0, posX, pantY);  //Linea vertical
  line(0, posY, pantX, posY);  //Linea horizontal

  //Dibuja el rectangulo en verde
  stroke(0, 255, 0);
  noFill();
  rect(posX - tamX/2, posY - tamY/2, tamX, tamY);

  //Establece el tamaño de la fuente a 1/4 de la altura de la barra
  textSize(alturaBarra/4);

  //Dibuja el boton del acelerometro
  fill(96);
  stroke(192);
  rect(0, pantY, anchoBoton, alturaBarra);
  if (accelActivo) fill(255); else fill(0);
  textAlign(CENTER, CENTER);
  text("Acelerómetro", 0, pantY, anchoBoton, alturaBarra);

  //Dibuja el boton de las opciones de conexion
  fill(96);
  stroke(192);
  rect(anchoBoton, pantY, anchoBoton, alturaBarra);
  fill(0);
  textAlign(CENTER, CENTER);
  text("Conexión...", anchoBoton, pantY, anchoBoton, alturaBarra);
}
