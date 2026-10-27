// AirtekLab - Programacion con millis() y ESP32
// Ejercicio 03: lectura periodica de temperatura y humedad con DHT22
// Placa: ESP32 Dev Module
// Sensor: DHT22
// Conexion sugerida: DATA al GPIO 4, VCC a 3V3 y GND a GND
// Requiere la biblioteca "DHT sensor library" de Adafruit.

#include <DHT.h>

const int PIN_DHT = 4;
const unsigned long INTERVALO_LECTURA = 2000;

DHT sensorDHT(PIN_DHT, DHT22);

unsigned long tiempoLecturaAnterior = 0;

void setup() {
  Serial.begin(115200);
  sensorDHT.begin();

  Serial.println("Ejercicio 03 - Lectura periodica del DHT22");
}

void loop() {
  unsigned long tiempoActual = millis();

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

  // El ESP32 puede ejecutar otras tareas aqui mientras espera
  // la siguiente lectura programada.
}
