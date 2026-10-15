/*
  IoT-JARVIS - ESP-NOW
  Etapa 02: DS18B20 -> ESP32 -> ESP-NOW
  Placa transmisora: ESP32 Dev Module

  Basado en el código del Capítulo 2 de AirtekLab.
  Se conserva el LCD I2C y se agrega el envío de temperatura.
*/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <WiFi.h>
#include <esp_now.h>

// MAC de la ESP32-C3 SuperMini receptora
uint8_t macReceptor[] = {0x44, 0xBD, 0x8D, 0x25, 0xA2, 0x14};

// LCD I2C: dirección 0x27, 16 columnas x 2 filas
LiquidCrystal_I2C lcd(0x27, 16, 2);

// DS18B20 conectado al GPIO 4, como en el Capítulo 2
#define ONE_WIRE_BUS 4
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensores(&oneWire);

float temperatura = 0.0;

void setup() {
  Serial.begin(115200);

  // Inicialización del LCD conservada del código del Capítulo 2
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Temperatura:");

  // Inicializamos el DS18B20
  sensores.begin();

  // Inicializamos Wi-Fi para ESP-NOW
  WiFi.mode(WIFI_STA);
  delay(500);

  Serial.print("MAC del transmisor: ");
  Serial.println(WiFi.STA.macAddress());

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error al iniciar ESP-NOW");
    lcd.setCursor(0, 1);
    lcd.print("Error ESP-NOW");
    while (true) {
      delay(1000);
    }
  }

  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, macReceptor, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Error al agregar el receptor");
    lcd.setCursor(0, 1);
    lcd.print("Error receptor");
    while (true) {
      delay(1000);
    }
  }

  Serial.println("ESP-NOW listo. Leyendo DS18B20...");
}

void loop() {
  // Solicitamos y leemos una medición del DS18B20
  sensores.requestTemperatures();
  temperatura = sensores.getTempCByIndex(0);

  // Comprobamos si el sensor respondió
  if (temperatura == DEVICE_DISCONNECTED_C) {
    Serial.println("Error: DS18B20 no detectado");

    lcd.setCursor(0, 1);
    lcd.print("Error sensor    ");

    delay(250);
    return;
  }

  // Mostramos la temperatura en el LCD
  lcd.setCursor(0, 1);
  lcd.print("                ");
  lcd.setCursor(0, 1);
  lcd.print(temperatura, 1);
  lcd.print((char)223);
  lcd.print("C");

  // Mostramos la temperatura en el Monitor Serie
  Serial.print("Temperatura medida: ");
  Serial.print(temperatura, 2);
  Serial.println(" C");

  // Enviamos el valor float al ESP32-C3 receptor
  esp_err_t resultado = esp_now_send(
    macReceptor,
    (uint8_t *)&temperatura,
    sizeof(temperatura)
  );

  if (resultado == ESP_OK) {
    Serial.println("Temperatura enviada");
  } else {
    Serial.print("Error de envio ESP-NOW: ");
    Serial.println(resultado);
  }

  delay(250);
}
