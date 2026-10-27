// AirtekLab - Programacion con millis() y ESP32
// Ejercicio 02: ejecutar dos tareas con intervalos independientes
// Placa: ESP32 Dev Module
// LED externo: GPIO 25 con resistencia limitadora
// Monitor Serie: 115200 baudios

const int PIN_LED = 25;

const unsigned long INTERVALO_LED = 1000;
const unsigned long INTERVALO_MENSAJE = 2500;

unsigned long tiempoLEDAnterior = 0;
unsigned long tiempoMensajeAnterior = 0;

bool estadoLED = LOW;

void setup() {
  Serial.begin(115200);

  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, estadoLED);

  Serial.println("Ejercicio 02 - Dos tareas independientes");
}

void loop() {
  unsigned long tiempoActual = millis();

  // Tarea 1: cambiar el estado del LED cada segundo.
  if (tiempoActual - tiempoLEDAnterior >= INTERVALO_LED) {
    tiempoLEDAnterior = tiempoActual;

    estadoLED = !estadoLED;
    digitalWrite(PIN_LED, estadoLED);
  }

  // Tarea 2: mostrar un mensaje cada 2,5 segundos.
  if (tiempoActual - tiempoMensajeAnterior >= INTERVALO_MENSAJE) {
    tiempoMensajeAnterior = tiempoActual;

    Serial.println("Tarea 2: mensaje periodico");
  }
}
