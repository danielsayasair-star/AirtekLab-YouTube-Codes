/*
  IoT-JARVIS - ESP-NOW
  Prueba básica: receptor
  Placa: ESP32-C3 SuperMini

  En el ESP32-C3, si el Monitor Serie no muestra los mensajes:
  Herramientas > USB CDC On Boot > Enabled
*/

#include <WiFi.h>
#include <esp_now.h>
#include <esp_arduino_version.h>

volatile int numeroRecibido = 0;
volatile bool hayDatoNuevo = false;

void procesarDato(const uint8_t *datos, int longitud) {
  if (longitud != sizeof(int)) {
    return;
  }

  int valor;
  memcpy(&valor, datos, sizeof(valor));
  numeroRecibido = valor;
  hayDatoNuevo = true;
}

// La firma del callback cambió entre Arduino-ESP32 2.x y 3.x.
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
  Serial.println(WiFi.macAddress());

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error al iniciar ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(alRecibir);
  Serial.println("Receptor ESP-NOW listo. Esperando numeros...");
}

void loop() {
  if (hayDatoNuevo) {
    int valor = numeroRecibido;
    hayDatoNuevo = false;

    Serial.print("Numero recibido: ");
    Serial.println(valor);
  }
}
