// AirtekLab - Programacion de pulsadores con ESP32
// Ejercicio 01: pulsador momentaneo con LED
// Placa: ESP32 Dev Module
// Pulsador: GPIO 18 a GND (INPUT_PULLUP)
// LED: GPIO 25 mediante resistencia limitadora a GND
// Nota: primer ejemplo didactico, sin antirrebote.

const int PIN_PULSADOR = 18;
const int PIN_LED = 25;

int estadoAnterior = HIGH;

void setup() {
  Serial.begin(115200);

  pinMode(PIN_PULSADOR, INPUT_PULLUP);
  pinMode(PIN_LED, OUTPUT);

  digitalWrite(PIN_LED, LOW);

  Serial.println("Sistema iniciado");
  Serial.println("Esperando pulsador...");
}

void loop() {
  int estadoActual = digitalRead(PIN_PULSADOR);

  if (estadoActual == LOW) {
    digitalWrite(PIN_LED, HIGH);
  } else {
    digitalWrite(PIN_LED, LOW);
  }

  if (estadoActual != estadoAnterior) {
    if (estadoActual == LOW) {
      Serial.println("01 - PULSADOR PRESIONADO");
    } else {
      Serial.println("02 - PULSADOR LIBERADO");
    }

    estadoAnterior = estadoActual;
  }
}
