// AirtekLab - Programacion de pulsadores con ESP32
// Ejercicio 04: contador de pulsaciones
// Placa: ESP32 Dev Module
// Pulsador: GPIO 18 a GND
// Monitor Serie: 115200 baudios
// Cada pulsacion valida suma una unidad al contador.

const int PIN_PULSADOR = 18;
const unsigned long TIEMPO_ANTIRREBOTE = 40;

int lecturaAnterior = HIGH;
int estadoEstable = HIGH;
unsigned long momentoCambio = 0;

unsigned long contadorPulsaciones = 0;

void setup() {
  Serial.begin(115200);

  pinMode(PIN_PULSADOR, INPUT_PULLUP);

  Serial.println("Ejercicio 04 - Contador de pulsaciones");
  Serial.println("Presiona el pulsador conectado al GPIO 18");
  Serial.println("Pulsaciones registradas: 0");
}

void loop() {
  int lectura = digitalRead(PIN_PULSADOR);

  // Detectamos si la lectura electrica cambio.
  if (lectura != lecturaAnterior) {
    lecturaAnterior = lectura;
    momentoCambio = millis();
  }

  // Aceptamos el nuevo estado cuando se mantuvo estable 40 ms.
  if ((millis() - momentoCambio) >= TIEMPO_ANTIRREBOTE) {
    if (lectura != estadoEstable) {
      estadoEstable = lectura;

      // Contamos solamente al presionar, no al soltar.
      if (estadoEstable == LOW) {
        contadorPulsaciones++;

        Serial.print("Pulsaciones registradas: ");
        Serial.println(contadorPulsaciones);
      }
    }
  }
}
