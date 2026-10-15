/*
  IoT-JARVIS - ESP-NOW
  Etapa 03: control de cooler de 12 V según temperatura recibida
  Placa receptora/controladora: ESP32-C3 SuperMini

  Base: receptor validado de la etapa 02.
  El módulo de relé se alimenta con una fuente externa de 5 V.
  No alimentar el cooler desde la ESP32-C3.
*/

#include <WiFi.h>
#include <esp_now.h>
#include <esp_arduino_version.h>

// NUEVO: GPIO conectado a IN1 del módulo de relé
#define PIN_RELE 5

// NUEVO: Ajustar después de comprobar la lógica del módulo.
// true = el relé se activa con HIGH; false = se activa con LOW.
const bool RELE_ACTIVO_ALTO = true;

// NUEVO: Umbrales de control con histéresis para evitar conmutaciones repetidas.
// Cooler se enciende al llegar a 25 °C y se apaga al bajar a 24 °C.
const float TEMPERATURA_ENCENDIDO = 25.0;
const float TEMPERATURA_APAGADO = 24.0;

// NUEVO: Si no llegan datos durante este tiempo, apagar el cooler por seguridad.
const unsigned long TIEMPO_MAX_SIN_DATOS_MS = 5000;

volatile float temperaturaRecibida = 0.0;
volatile bool hayDatoNuevo = false;

// NUEVO: Estado de control del cooler
bool coolerEncendido = false;
unsigned long ultimoDatoMs = 0;
bool seRecibioAlMenosUnDato = false;

void establecerCooler(bool encender) {
  coolerEncendido = encender;

  // NUEVO: Aplicar el nivel eléctrico según la polaridad configurada.
  bool nivelAlto = (encender == RELE_ACTIVO_ALTO);
  digitalWrite(PIN_RELE, nivelAlto ? HIGH : LOW);

  Serial.print("Cooler: ");
  Serial.println(encender ? "ENCENDIDO" : "APAGADO");
}

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

  // NUEVO: Inicializar la salida en estado apagado antes de iniciar la radio.
  pinMode(PIN_RELE, OUTPUT);
  digitalWrite(PIN_RELE, RELE_ACTIVO_ALTO ? LOW : HIGH);

  WiFi.mode(WIFI_STA);
  delay(500);

  Serial.print("MAC del receptor: ");
  Serial.println(WiFi.STA.macAddress());

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error al iniciar ESP-NOW. Cooler mantenido apagado.");
    while (true) {
      delay(1000);
    }
  }

  esp_now_register_recv_cb(alRecibir);
  Serial.println("Receptor listo. Esperando temperatura...");
  Serial.println("Control: cooler ON >= 25.0 C; OFF <= 24.0 C.");
}

void loop() {
  if (hayDatoNuevo) {
    // Copiar el dato recibido y liberar la bandera.
    float valor = temperaturaRecibida;
    hayDatoNuevo = false;

    ultimoDatoMs = millis();
    seRecibioAlMenosUnDato = true;

    Serial.print("Temperatura recibida: ");
    Serial.print(valor, 2);
    Serial.println(" C");

    // NUEVO: Control con histéresis.
    if (!coolerEncendido && valor >= TEMPERATURA_ENCENDIDO) {
      establecerCooler(true);
    } else if (coolerEncendido && valor <= TEMPERATURA_APAGADO) {
      establecerCooler(false);
    }
  }

  // NUEVO: Si se pierde la comunicación, apagar el cooler.
  if (seRecibioAlMenosUnDato &&
      coolerEncendido &&
      millis() - ultimoDatoMs > TIEMPO_MAX_SIN_DATOS_MS) {
    Serial.println("Sin datos de temperatura durante 5 s: apagado de seguridad.");
    establecerCooler(false);
  }
}
