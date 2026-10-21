// AirtekLab - Programacion de pulsadores con ESP32
// Ejercicio 06: distinguir pulsacion corta y larga
// Placa: ESP32 Dev Module
// Pulsador: GPIO 18 a GND
// Monitor Serie: 115200 baudios
// La accion se informa al soltar el pulsador.

const int PIN_PULSADOR = 18;
const unsigned long TIEMPO_ANTIRREBOTE = 40;
const unsigned long UMBRAL_PULSACION_LARGA = 800;

int lecturaAnterior = HIGH;
int estadoEstable = HIGH;
unsigned long momentoCambio = 0;

unsigned long momentoPulsacion = 0;

void setup() {
  Serial.begin(115200);
  pinMode(PIN_PULSADOR, INPUT_PULLUP);

  Serial.println("Ejercicio 06 - Pulsacion corta y larga");
  Serial.println("Pulsa menos de 800 ms: PULSACION CORTA");
  Serial.println("Mantene pulsado 800 ms o mas: PULSACION LARGA");
}

void loop() {
  int lectura = digitalRead(PIN_PULSADOR);

  // Detectamos cambios en la lectura del pulsador.
  if (lectura != lecturaAnterior) {
    lecturaAnterior = lectura;
    momentoCambio = millis();
  }

  // Aceptamos el cambio solo si la lectura se mantuvo estable 40 ms.
  if ((millis() - momentoCambio) >= TIEMPO_ANTIRREBOTE) {
    if (lectura != estadoEstable) {
      estadoEstable = lectura;

      if (estadoEstable == LOW) {
        // NUEVO: registramos el momento en que comienza la pulsacion.
        momentoPulsacion = millis();
      } else {
        // NUEVO: al soltar, calculamos cuanto tiempo estuvo pulsado.
        unsigned long duracionPulsacion = millis() - momentoPulsacion;

        if (duracionPulsacion >= UMBRAL_PULSACION_LARGA) {
          Serial.print("PULSACION LARGA - Duracion: ");
          Serial.print(duracionPulsacion);
          Serial.println(" ms - ACCION 2");
        } else {
          Serial.print("PULSACION CORTA - Duracion: ");
          Serial.print(duracionPulsacion);
          Serial.println(" ms - ACCION 1");
        }
      }
    }
  }
}
