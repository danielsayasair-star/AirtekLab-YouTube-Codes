/*
  IoT-JARVIS - ESP-NOW
  Obtener la MAC de estación (STA)
  Compatible con Arduino-ESP32 3.x, probado como corrección
  propuesta para la versión 3.3.12.
*/

#include <WiFi.h>

void setup() {
  Serial.begin(115200);
  delay(500);

  // Inicia explícitamente la interfaz Wi-Fi de estación.
  if (!WiFi.STA.begin()) {
    Serial.println("Error al iniciar la interfaz Wi-Fi STA");
    return;
  }

  // Consultamos la MAC de la interfaz STA, la que usamos en ESP-NOW.
  Serial.print("MAC de estación (STA): ");
  Serial.println(WiFi.STA.macAddress());
}

void loop() {
}
