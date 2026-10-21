// AirtekLab - Programacion de pulsadores con ESP32
// Ejercicio 05: modificar una consigna
// Placa: ESP32 Dev Module
// Pulsador: GPIO 18 a GND
// Monitor Serie: 115200 baudios
// Una pulsacion suma una unidad; mantener pulsado repite el incremento.

const int PIN_PULSADOR = 18;
const unsigned long TIEMPO_ANTIRREBOTE = 40;
const unsigned long ESPERA_REPETICION = 600;
const unsigned long INTERVALO_REPETICION = 150;

int lecturaAnterior = HIGH;
int estadoEstable = HIGH;
unsigned long momentoCambio = 0;

int consigna = 20;
unsigned long momentoInicioPulsacion = 0;
unsigned long momentoUltimoIncremento = 0;

void aumentarConsigna() {
  consigna++;
  Serial.print("Nueva consigna: ");
  Serial.println(consigna);
}

void setup() {
  Serial.begin(115200);
  pinMode(PIN_PULSADOR, INPUT_PULLUP);

  Serial.println("Ejercicio 05 - Consigna con pulsador");
  Serial.print("Consigna inicial: ");
  Serial.println(consigna);
  Serial.println("Pulsa para aumentar; manten pulsado para repetir.");
}

void loop() {
  int lectura = digitalRead(PIN_PULSADOR);

  if (lectura != lecturaAnterior) {
    lecturaAnterior = lectura;
    momentoCambio = millis();
  }

  if ((millis() - momentoCambio) >= TIEMPO_ANTIRREBOTE) {
    if (lectura != estadoEstable) {
      estadoEstable = lectura;

      if (estadoEstable == LOW) {
        aumentarConsigna();
        momentoInicioPulsacion = millis();
        momentoUltimoIncremento = millis();
      }
    }
  }

  if (estadoEstable == LOW) {
    unsigned long ahora = millis();

    if ((ahora - momentoInicioPulsacion) >= ESPERA_REPETICION &&
        (ahora - momentoUltimoIncremento) >= INTERVALO_REPETICION) {
      aumentarConsigna();
      momentoUltimoIncremento = ahora;
    }
  }
}
