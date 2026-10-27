// AirtekLab - Programacion con millis() y ESP32
// Ejercicio 01: parpadear un LED sin delay()
// Placa: ESP32 Dev Module
// LED externo: GPIO 25 con resistencia limitadora

const int PIN_LED = 25;

unsigned long tiempoAnterior = 0;
const unsigned long INTERVALO = 1000;

bool estadoLED = LOW;

void setup() {
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, estadoLED);
}

void loop() {
  unsigned long tiempoActual = millis();

  if (tiempoActual - tiempoAnterior >= INTERVALO) {
    tiempoAnterior = tiempoActual;

    estadoLED = !estadoLED;
    digitalWrite(PIN_LED, estadoLED);
  }
}
