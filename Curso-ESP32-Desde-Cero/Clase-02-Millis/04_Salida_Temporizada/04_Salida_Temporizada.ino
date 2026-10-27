// AirtekLab - Programacion con millis() y ESP32
// Ejercicio 04: activar una salida durante un tiempo determinado
// Placa: ESP32 Dev Module
// LED: GPIO 25 con resistencia limitadora
// DHT22: DATA en GPIO 4
// Monitor Serie: 115200 baudios
// Enviar '1' para activar el LED durante 3 segundos.
// Enviar '0' para apagarlo inmediatamente.

#include <DHT.h>

const int PIN_LED = 25;
const int PIN_DHT = 4;

const unsigned long INTERVALO_LECTURA = 2000;
const unsigned long DURACION_SALIDA = 3000;

DHT sensorDHT(PIN_DHT, DHT22);

unsigned long tiempoLecturaAnterior = 0;
unsigned long momentoActivacion = 0;

bool salidaActiva = false;

void setup() {
  Serial.begin(115200);

  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, LOW);

  sensorDHT.begin();

  Serial.println("Ejercicio 04 - Salida temporizada");
  Serial.println("Enviar 1: activar LED durante 3 segundos");
  Serial.println("Enviar 0: apagar LED inmediatamente");
}

void loop() {
  unsigned long tiempoActual = millis();

  // Tarea 1: leer temperatura y humedad cada 2 segundos.
  if (tiempoActual - tiempoLecturaAnterior >= INTERVALO_LECTURA) {
    tiempoLecturaAnterior = tiempoActual;

    float humedad = sensorDHT.readHumidity();
    float temperatura = sensorDHT.readTemperature();

    if (isnan(humedad) || isnan(temperatura)) {
      Serial.println("Error: no se pudo leer el DHT22");
    } else {
      Serial.print("Temperatura: ");
      Serial.print(temperatura, 1);
      Serial.print(" C | Humedad: ");
      Serial.print(humedad, 1);
      Serial.println(" %");
    }
  }

  // Tarea 2: recibir comandos sin esperar con delay().
  if (Serial.available() > 0) {
    char comando = Serial.read();

    if (comando == '1') {
      salidaActiva = true;
      momentoActivacion = millis();
      digitalWrite(PIN_LED, HIGH);
      Serial.println("Salida activada por 3 segundos");
    } else if (comando == '0') {
      salidaActiva = false;
      digitalWrite(PIN_LED, LOW);
      Serial.println("Salida apagada manualmente");
    }
  }

  // Tarea 3: apagar la salida cuando se cumple el tiempo.
  if (salidaActiva && (millis() - momentoActivacion >= DURACION_SALIDA)) {
    salidaActiva = false;
    digitalWrite(PIN_LED, LOW);
    Serial.println("Tiempo cumplido: salida apagada");
  }
}
