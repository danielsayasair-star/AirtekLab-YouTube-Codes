/*
   Sistema de Monitoreo de Temperatura WiFi
   Capítulo 4
   Servidor Web con ESP32
*/

//====================================================
// LIBRERÍAS
//====================================================

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <WiFi.h>

// NUEVO:
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

// NUEVO:
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
// PÁGINA WEB
//====================================================

// NUEVO:
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

<div class="temperatura">

%TEMPERATURA%

</div>

</body>

</html>

)rawliteral";

//====================================================
// PÁGINA PRINCIPAL
//====================================================

// NUEVO:
// Esta función será ejecutada cada vez que un
// navegador acceda a la dirección IP del ESP32.
void paginaPrincipal() {

  // NUEVO:
  // Creamos una copia del HTML para poder modificar
  // el valor de la temperatura antes de enviarlo.
  String html = pagina;

  // NUEVO:
  // Reemplazamos el marcador por la temperatura actual.
  html.replace("%TEMPERATURA%", String(temperatura, 1) + " &deg;C");

  // NUEVO:
  // Enviamos la página al navegador.
  servidor.send(200, "text/html", html);

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

  // NUEVO:
  // Iniciamos la conexión a la red WiFi.
  WiFi.begin(ssid, password);

  Serial.print("Conectando");

  // NUEVO:
  // Esperamos hasta que el ESP32 se conecte.
  while (WiFi.status() != WL_CONNECTED) {

    delay(500);
    Serial.print(".");

  }

  Serial.println();
  Serial.println();

  Serial.println("WiFi conectado");

  // NUEVO:
  // Mostramos la dirección IP asignada.
  Serial.print("Direccion IP: ");
  Serial.println(WiFi.localIP());

  //====================================================
  // SERVIDOR WEB
  //====================================================

  // NUEVO:
  // Cuando un navegador solicite la dirección "/"
  // se ejecutará la función paginaPrincipal().
  servidor.on("/", paginaPrincipal);

  // NUEVO:
  // Iniciamos el servidor web.
  servidor.begin();

  Serial.println("Servidor Web iniciado");

}

//====================================================
// LOOP
//====================================================

void loop() {

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
  // SERVIDOR WEB
  //====================================================

  // NUEVO:
  // Atiende las solicitudes que llegan desde
  // cualquier navegador conectado a la red.
  servidor.handleClient();

  // Esperamos un segundo.
  delay(1000);

}
