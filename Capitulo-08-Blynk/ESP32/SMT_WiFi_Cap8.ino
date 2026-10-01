/*
   Sistema de Monitoreo de Temperatura WiFi
   Capítulo 8
   ESP32 + Blynk
*/

//====================================================
// NUEVO — BLYNK
//====================================================

#define BLYNK_TEMPLATE_ID "TU_TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "Monitor de Temperatura"
#define BLYNK_AUTH_TOKEN "TU_AUTH_TOKEN"

#include <BlynkSimpleEsp32.h>

//====================================================
// LIBRERÍAS
//====================================================

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <WiFi.h>

// Librería para crear un servidor web.
#include <WebServer.h>

//====================================================
// CONFIGURACIÓN WIFI
//====================================================

const char* ssid = "TU_WIFI";
const char* password = "TU_PASSWORD";

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

//====================================================
// NUEVO — VARIABLE PARA COMPARAR TEMPERATURAS
//====================================================

float ultimaTemperatura = 0;
bool primeraMedicion = true;

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

// Esta función consulta la temperatura al ESP32.
function actualizarTemperatura() {

  fetch("/temperatura")

    .then(respuesta => respuesta.text())

    .then(dato => {

      document.getElementById("temperatura").innerHTML =
        dato + " &deg;C";

    });

}

// Ejecutamos la función cada 1000 milisegundos,
// es decir, una vez por segundo.
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

  // Creamos una copia del HTML para poder modificar
  // el valor de la temperatura antes de enviarlo.
  String html = pagina;

  // Reemplazamos el marcador por la temperatura actual.
  html.replace("%TEMPERATURA%", String(temperatura, 1) + " &deg;C");

  // Enviamos la página al navegador.
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

  // Iniciamos el Monitor Serie.
  Serial.begin(115200);

  // Inicializamos el display.
  lcd.init();

  // Encendemos la retroiluminación.
  lcd.backlight();

  // Inicializamos el sensor.
  sensores.begin();

  // Escribimos el texto fijo.
  lcd.setCursor(0, 0);
  lcd.print("Temperatura:");

  //====================================================
  // CONEXIÓN WIFI
  //====================================================

  // Iniciamos la conexión a la red WiFi.
  WiFi.begin(ssid, password);

  Serial.print("Conectando");

  // Esperamos hasta que el ESP32 se conecte.
  while (WiFi.status() != WL_CONNECTED) {

    delay(500);
    Serial.print(".");

  }

  Serial.println();
  Serial.println();

  Serial.println("WiFi conectado");

  // Mostramos la dirección IP asignada.
  Serial.print("Direccion IP: ");
  Serial.println(WiFi.localIP());

  //====================================================
  // SERVIDOR WEB
  //====================================================

  // Cuando un navegador solicite la dirección "/"
  // se ejecutará la función paginaPrincipal().
  servidor.on("/", paginaPrincipal);

  // Cuando el navegador solicite "/temperatura",
  // el ESP32 responderá solamente con el valor actual.
  servidor.on("/temperatura", enviarTemperatura);

  // Iniciamos el servidor web.
  servidor.begin();

  Serial.println("Servidor Web iniciado");

  //====================================================
  // NUEVO — CONEXIÓN CON BLYNK
  //====================================================

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password);

}

//====================================================
// LOOP
//====================================================

void loop() {

  //====================================================
  // NUEVO — MANTENER CONEXIÓN CON BLYNK
  //====================================================

  Blynk.run();

  // Solicitamos una nueva medición.
  sensores.requestTemperatures();

  // Leemos la temperatura del primer sensor.
  temperatura = sensores.getTempCByIndex(0);

  // Mostramos la temperatura en el display.
  lcd.setCursor(0, 1);

  // Borramos la línea para evitar que queden
  // caracteres de una lectura anterior.
  lcd.print("                ");

  // Volvemos al inicio de la línea.
  lcd.setCursor(0, 1);

  // Mostramos la temperatura.
  lcd.print(temperatura, 1);

  // Símbolo de grados.
  lcd.print((char)223);

  // Grados Celsius.
  lcd.print("C");

  // Enviamos el dato al Monitor Serie.
  Serial.print("Temperatura: ");
  Serial.print(temperatura);
  Serial.println(" C");

  //====================================================
  // NUEVO — ENVIAR TEMPERATURA A BLYNK
  //====================================================

  // Enviamos la primera medición.
  // Después solamente enviamos si la temperatura cambió.
  if (primeraMedicion || temperatura != ultimaTemperatura) {

    Blynk.virtualWrite(V0, temperatura);

    ultimaTemperatura = temperatura;
    primeraMedicion = false;

  }

  //====================================================
  // SERVIDOR WEB
  //====================================================

  // Atiende las solicitudes que llegan desde
  // cualquier navegador conectado a la red.
  servidor.handleClient();

  // Esperamos un segundo.
  delay(1000);

}
