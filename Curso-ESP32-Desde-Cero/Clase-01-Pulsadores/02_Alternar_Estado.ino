// AirtekLab - Programacion de pulsadores con ESP32
// Ejercicio 02: cada pulsacion alterna el estado del LED
// Placa: ESP32 Dev Module
// Pulsador: GPIO 18 a GND (INPUT_PULLUP)
// LED: GPIO 25 mediante resistencia limitadora a GND
// Antirrebote sencillo con millis().

const int PIN_PULSADOR = 18;
const int PIN_LED = 25;

const unsigned long TIEMPO_ANTIRREBOTE = 40;

int lecturaAnterior = HIGH;
int estadoEstable = HIGH;
unsigned long momentoUltimoCambio = 0;

bool ledActivo = false;

void setup() {
  Serial.begin(115200);

  pinMode(PIN_PULSADOR, INPUT_PULLUP);
  pinMode(PIN_LED, OUTPUT);

  digitalWrite(PIN_LED, LOW);

  Serial.println("Ejercicio 02 - Alternar estado");
  Serial.println("Cada pulsacion cambia el LED entre ON y OFF");
  Serial.println("Estado inicial: OFF");
}

void loop() {
  int lectura = digitalRead(PIN_PULSADOR);

  if (lectura != lecturaAnterior) {
    lecturaAnterior = lectura;
    momentoUltimoCambio = millis();
  }

  if ((millis() - momentoUltimoCambio) >= TIEMPO_ANTIRREBOTE) {
    if (lectura != estadoEstable) {
      estadoEstable = lectura;

      // Una nueva pulsacion se detecta cuando la entrada pasa a LOW.
      if (estadoEstable == LOW) {
        ledActivo = !ledActivo;

        if (ledActivo) {
          digitalWrite(PIN_LED, HIGH);
          Serial.println("01 - LED ACTIVADO");
        } else {
          digitalWrite(PIN_LED, LOW);
          Serial.println("02 - LED DESACTIVADO");
        }
      }
    }
  }
}
