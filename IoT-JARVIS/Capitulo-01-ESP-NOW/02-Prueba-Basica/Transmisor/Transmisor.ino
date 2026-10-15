/*
  IoT-JARVIS - ESP-NOW
  Prueba básica: transmisor
  Placa: ESP32 Dev Module
*/

#include <WiFi.h>
#include <esp_now.h>

// MAC de la ESP32-C3 SuperMini receptora
uint8_t macReceptor[] = {0x44, 0xBD, 0x8D, 0x25, 0xA2, 0x14};

int numero = 0;

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);

  Serial.print("MAC del transmisor: ");
  Serial.println(WiFi.macAddress());

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error al iniciar ESP-NOW");
    return;
  }

  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, macReceptor, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Error al agregar el receptor");
    return;
  }

  Serial.println("ESP-NOW listo. Enviando numeros...");
}

void loop() {
  numero++;

  esp_err_t resultado = esp_now_send(
    macReceptor,
    (uint8_t *)&numero,
    sizeof(numero)
  );

  if (resultado == ESP_OK) {
    Serial.print("Enviado: ");
    Serial.println(numero);
  } else {
    Serial.print("Error de envio: ");
    Serial.println(resultado);
  }

  delay(1000);
}
