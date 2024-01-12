#include <SoftwareSerial.h>

SoftwareSerial mySerial(0, 1); // RX | TX

// Declaración de variables
int command;
float intensity = 100;
bool lucesAltas = false;
bool lucesBajas = false;
bool lucesIntermitentes = false;
bool reversa = false;
bool tonoActivo = false;
unsigned long lastToggleTimeI = 0;  
unsigned long lastToggleTimeB = 0;  

// Declaración de Constantes
const long interval = 500;          // Intervalo de medio segundo (en milisegundos)
const long intervalB = 500;         // Intervalo de medio segundo (en milisegundos)

void setup() {
   Serial.begin(9600);
   mySerial.begin(9600);
   Serial.println("You're connected via Bluetooth");
   
   // LEDS
   pinMode(9, OUTPUT);
   pinMode(8, OUTPUT);

   // BUZZER
   pinMode(7, OUTPUT);

   // LLANTAS
   pinMode(13,OUTPUT);   //left motors forward
   pinMode(12,OUTPUT);   //left motors reverse
   pinMode(11,OUTPUT);   //right motors forward
   pinMode(10,OUTPUT);   //right motors reverse

}

void loop() {
  if (mySerial.available()) {
    command=(mySerial.read());
    if (command=='F') {             // Si F, adelante
      Serial.println("Forward");
      digitalWrite(13,HIGH);
      digitalWrite(12,LOW);
      digitalWrite(11,HIGH);
      digitalWrite(10,LOW);
      reversa = false;
      noTone(7);
    } 
    else if (command=='B') {        // Si B, Atras
      Serial.println("Reverse");
      digitalWrite(13,LOW);
      digitalWrite(12,HIGH);
      digitalWrite(11,LOW);
      digitalWrite(10,HIGH);
      tone(7, 1000);                // Sonido en el buzzer
      reversa = true;   
      lastToggleTimeB = millis();
    } else if (command=='L') {      // Si L, Izquierda
      Serial.println("Left");
      digitalWrite(13,LOW);
      digitalWrite(12,LOW);
      digitalWrite(11,HIGH);
      digitalWrite(10,LOW);
      reversa = false;
      noTone(7);
    } else if (command=='R') {      // Si R, Derecha
      Serial.println("Right");
      digitalWrite(13,HIGH);
      digitalWrite(12,LOW);
      digitalWrite(11,LOW);
      digitalWrite(10,LOW);
      reversa = false;
      noTone(7);
    } else if (command=='S') {      // Si S, Detener
      Serial.println("Stop");
      digitalWrite(13,LOW);
      digitalWrite(12,LOW);
      digitalWrite(11,LOW);
      digitalWrite(10,LOW);
      reversa = false;
      noTone(7);
    } else if (command == 'A') {    // Si A, Luces Altas
      Serial.println("Luces Altas");
      lucesAltas = !lucesAltas;  
      lucesBajas = false;
      digitalWrite(9, lucesAltas ? HIGH : LOW);
    }
    else if (command == 'U') {      // si U, Luces Bajas
      Serial.println("Luces Bajas");
      lucesBajas = !lucesBajas;  
      lucesAltas = false;
      analogWrite(9, lucesBajas ? intensity : LOW);
    }
    else if (command == 'I') {      // Si I, Luces intermitentes
      Serial.println("Luces Intermitentes");  
      lucesIntermitentes = !lucesIntermitentes;  
      if (lucesIntermitentes == 0) {
        digitalWrite(8, LOW);
      }
      // Verificar si ha pasado el intervalo de tiempo
      lastToggleTimeI = millis();
    }
  }

  // funcionamiento de las luces intermitentes
  if (lucesIntermitentes) {
    unsigned long currentMillisI = millis();
    if (currentMillisI - lastToggleTimeI >= interval) {
      // Guardar el tiempo actual
      lastToggleTimeI = currentMillisI;
      // Cambiar el estado de los LED
      digitalWrite(8, lucesIntermitentes ? !digitalRead(8) : LOW);
    }
  }

  // Funcionamiento del sonido en el buzzer para la reversa
  if (reversa) {
    unsigned long currentMillisB = millis();
    if (currentMillisB - lastToggleTimeB >= intervalB) {
      // Guardar el tiempo actual
      lastToggleTimeB = currentMillisB;
      // Cambiar el estado de los LED
      // Cambiar el estado del tono
        if (tonoActivo) {
            noTone(7);
        } else {
            tone(7, 1000);
        }
        // Invertir el estado del tono
        tonoActivo = !tonoActivo;
    }
  }
  delay(30);
}