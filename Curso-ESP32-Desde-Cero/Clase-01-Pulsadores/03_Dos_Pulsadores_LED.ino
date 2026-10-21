// AirtekLab - Programacion de pulsadores con ESP32
// Ejercicio 03: dos pulsadores para encender y apagar un LED
// Placa: ESP32 Dev Module
//
// Pulsador ENCENDER: GPIO 18 a GND
// Pulsador APAGAR: GPIO 19 a GND
// LED: GPIO 25 mediante resistencia limitadora a GND

const int PIN_PULSADOR_ENCENDER = 18;
const int PIN_PULSADOR_APAGAR = 19;
const int PIN_LED = 25;

const unsigned long TIEMPO_ANTIRREBOTE = 40;

int lecturaAnteriorEncender = HIGH;
int estadoEstableEncender = HIGH;
unsigned long momentoCambioEncender = 0;

int lecturaAnteriorApagar = HIGH;
int estadoEstableApagar = HIGH;
unsigned long momentoCambioApagar = 0;

bool ledActivo = false;

void actualizarLed() {
  digitalWrite(PIN_LED, ledActivo ? HIGH : LOW);
}

void setup() {
  Serial.begin(115200);

  pinMode(PIN_PULSADOR_ENCENDER, INPUT_PULLUP);
  pinMode(PIN_PULSADOR_APAGAR, INPUT_PULLUP);
  pinMode(PIN_LED, OUTPUT);

  ledActivo = false;
  actualizarLed();

  Serial.println("Ejercicio 03 - Dos pulsadores y un LED");
  Serial.println("Pulsador GPIO 18: ENCENDER LED");
  Serial.println("Pulsador GPIO 19: APAGAR LED");
  Serial.println("Antirrebote: 40 ms");
  Serial.println("Estado inicial: LED APAGADO");
}

void loop() {
  int lecturaEncender = digitalRead(PIN_PULSADOR_ENCENDER);
  int lecturaApagar = digitalRead(PIN_PULSADOR_APAGAR);

  // --- ANTIRREBOTE DEL PULSADOR ENCENDER ---
  if (lecturaEncender != lecturaAnteriorEncender) {
    lecturaAnteriorEncender = lecturaEncender;
    momentoCambioEncender = millis();
  }

  if ((millis() - momentoCambioEncender) >= TIEMPO_ANTIRREBOTE) {
    if (lecturaEncender != estadoEstableEncender) {
      estadoEstableEncender = lecturaEncender;

      if (estadoEstableEncender == LOW) {
        ledActivo = true;
        actualizarLed();
        Serial.println("01 - LED ENCENDIDO");
      }
    }
  }

  // --- ANTIRREBOTE DEL PULSADOR APAGAR ---
  if (lecturaApagar != lecturaAnteriorApagar) {
    lecturaAnteriorApagar = lecturaApagar;
    momentoCambioApagar = millis();
  }

  if ((millis() - momentoCambioApagar) >= TIEMPO_ANTIRREBOTE) {
    if (lecturaApagar != estadoEstableApagar) {
      estadoEstableApagar = lecturaApagar;

      if (estadoEstableApagar == LOW) {
        ledActivo = false;
        actualizarLed();
        Serial.println("02 - LED APAGADO");
      }
    }
  }
}
