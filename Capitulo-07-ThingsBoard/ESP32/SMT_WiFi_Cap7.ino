/*
   Sistema de Monitoreo de Temperatura WiFi
   Capítulo 7
   ESP32 + ThingsBoard
*/

//====================================================
// LIBRERÍAS
//====================================================

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <WiFi.h>
#include <WebServer.h>

// NUEVO:
// Librerías para comunicación con ThingsBoard mediante MQTT.
#include <Arduino_MQTT_Client.h>
#include <ThingsBoard.h>

//====================================================
// CONFIGURACIÓN WIFI
//====================================================

const char* ssid = "RED";
const char* password = "CONTRASEÑA";

//====================================================
// SERVIDOR WEB
//====================================================

// Creamos un servidor web utilizando el puerto 80.
WebServer servidor(80);

//====================================================
// LCD
//====================================================

LiquidCrystal_I2C lcd(0x27, 16, 2);

//====================================================
// SENSOR DS18B20
//====================================================

#define ONE_WIRE_BUS 4

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensores(&oneWire);

//====================================================
// VARIABLES
//====================================================

float temperatura = 0;

// NUEVO:
// Guarda la última temperatura enviada a ThingsBoard.
float ultimaTemperaturaEnviada = 0;

// NUEVO:
// Indica si todavía no se realizó ningún envío.
bool primerEnvio = true;

// NUEVO:
// Configuración de conexión con ThingsBoard.
const char* thingsboardServer = "mqtt.thingsboard.cloud";
const uint16_t thingsboardPort = 1883;
const char* token = "PEGA_TU_TOKEN_AQUI";

// NUEVO:
// Cliente de red que utilizará MQTT.
WiFiClient espClient;

// NUEVO:
// Cliente MQTT que utilizará la conexión WiFi.
Arduino_MQTT_Client mqttClient(espClient);

// NUEVO:
// Cliente ThingsBoard.
ThingsBoard tb(
  mqttClient,
  128,
  128,
  Default_Max_Stack_Size
);

// NUEVO:
// Conecta el ESP32 con ThingsBoard mediante MQTT.
void conectarThingsBoard() {

  Serial.println("Conectando con ThingsBoard...");

  if (!tb.connected()) {

    if (!tb.connect(thingsboardServer, token, thingsboardPort)) {

      Serial.println("Error al conectar con ThingsBoard");
      return;

    }

    Serial.println("ThingsBoard conectado");

  }

}

//====================================================
// PÁGINA WEB
//====================================================

// R"rawliteral" nos permite escribir el código HTML
// directamente dentro del programa.
const char pagina[] PROGMEM = R"rawliteral(

<!DOCTYPE html>

<html>

<head>

<meta charset="UTF-8">

<title>Monitor de Temperatura</title>

<style>

body{

font-family:Arial;
background:#f2f2f2;
text-align:center;
margin-top:60px;

}

h1{

color:#1565C0;

}

p{

font-size:24px;

}

.temperatura{

font-size:60px;
font-weight:bold;
color:#E53935;

}

</style>

</head>

<body>

<h1>Sistema de Monitoreo</h1>

<p>Temperatura actual</p>

<div class="temperatura"id="temperatura">

%TEMPERATURA%

</div>

<script>

function actualizarTemperatura() {

  fetch("/temperatura")

    .then(respuesta => respuesta.text())

    .then(dato => {

      document.getElementById("temperatura").innerHTML =
        dato + " &deg;C";

    });

}

setInterval(actualizarTemperatura, 1000);

</script>

</body>

</html>

)rawliteral";

//====================================================
// PÁGINA PRINCIPAL
//====================================================

// Esta función será ejecutada cada vez que un
// navegador acceda a la dirección IP del ESP32.
void paginaPrincipal() {

  String html = pagina;

  html.replace("%TEMPERATURA%", String(temperatura, 1) + " &deg;C");

  servidor.send(200, "text/html", html);

}

// Esta función envía solamente el valor actual
// de la temperatura al navegador.
void enviarTemperatura() {

  servidor.send(200, "text/plain", String(temperatura, 1));

}

//====================================================
// SETUP
//====================================================

void setup() {

  Serial.begin(115200);

  lcd.init();

  lcd.backlight();

  sensores.begin();

  lcd.setCursor(0, 0);
  lcd.print("Temperatura:");

  //====================================================
  // CONEXIÓN WIFI
  //====================================================

  WiFi.begin(ssid, password);

  Serial.print("Conectando");

  while (WiFi.status() != WL_CONNECTED) {

    delay(500);
    Serial.print(".");

  }

  Serial.println();
  Serial.println();

  Serial.println("WiFi conectado");

  Serial.print("Direccion IP: ");
  Serial.println(WiFi.localIP());

  //====================================================
  // SERVIDOR WEB
  //====================================================

  servidor.on("/", paginaPrincipal);

  servidor.on("/temperatura", enviarTemperatura);

  servidor.begin();

  Serial.println("Servidor Web iniciado");

  // NUEVO:
  // Conectamos con ThingsBoard después de iniciar el servidor web.
  conectarThingsBoard();

}

//====================================================
// LOOP
//====================================================

void loop() {

  // NUEVO:
  // Verificamos que la conexión con ThingsBoard siga activa.
  if (!tb.connected()) {
    conectarThingsBoard();
  }

  // Solicitamos una nueva medición.
  sensores.requestTemperatures();

  // Leemos la temperatura del primer sensor.
  temperatura = sensores.getTempCByIndex(0);

  // Mostramos la temperatura en el display.
  lcd.setCursor(0, 1);

  lcd.print("                ");

  lcd.setCursor(0, 1);

  lcd.print(temperatura, 1);

  lcd.print((char)223);

  lcd.print("C");

  // Enviamos el dato al Monitor Serie.
  Serial.print("Temperatura: ");
  Serial.print(temperatura);
  Serial.println(" C");

  // NUEVO:
  // Enviamos la temperatura a ThingsBoard solamente
  // cuando cambia respecto del último valor enviado.
  if (primerEnvio || temperatura != ultimaTemperaturaEnviada) {

    tb.sendTelemetryData("temperature", temperatura);

    ultimaTemperaturaEnviada = temperatura;

    primerEnvio = false;

  }

  //====================================================
  // SERVIDOR WEB
  //====================================================

  servidor.handleClient();

  delay(1000);

}
