/*
  IoT-JARVIS - ESP-NOW
  Etapa 02: ESP32-C3 recibe temperatura
  Placa receptora: ESP32-C3 SuperMini

  Si el Monitor Serie no muestra mensajes:
  Herramientas > USB CDC On Boot > Enabled
*/

#include <WiFi.h>
#include <esp_now.h>
#include <esp_arduino_version.h>

volatile float temperaturaRecibida = 0.0;
volatile bool hayDatoNuevo = false;

void procesarDato(const uint8_t *datos, int longitud) {
  if (longitud != sizeof(float)) {
    return;
  }

  float valor;
  memcpy(&valor, datos, sizeof(valor));
  temperaturaRecibida = valor;
  hayDatoNuevo = true;
}

// Compatibilidad con Arduino-ESP32 2.x y 3.x
#if ESP_ARDUINO_VERSION_MAJOR >= 3
void alRecibir(const esp_now_recv_info_t *info, const uint8_t *datos, int longitud) {
  procesarDato(datos, longitud);
}
#else
void alRecibir(const uint8_t *mac, const uint8_t *datos, int longitud) {
  procesarDato(datos, longitud);
}
#endif

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  delay(500);

  Serial.print("MAC del receptor: ");
  Serial.println(WiFi.STA.macAddress());

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error al iniciar ESP-NOW");
    while (true) {
      delay(1000);
    }
  }

  esp_now_register_recv_cb(alRecibir);
  Serial.println("Receptor listo. Esperando temperatura...");
}

void loop() {
  if (hayDatoNuevo) {
    float valor = temperaturaRecibida;
    hayDatoNuevo = false;

    Serial.print("Temperatura recibida: ");
    Serial.print(valor, 2);
    Serial.println(" C");
  }
}
