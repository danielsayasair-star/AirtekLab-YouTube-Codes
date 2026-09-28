/*
   Sistema de Monitoreo de Temperatura WiFi
   Capítulo 9
   ESP32 + Google Sheets + Alarmas
*/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <WiFi.h>
#include <time.h>
#include <HTTPClient.h>
#include <WebServer.h>

// COMPLETAR CON LOS DATOS DE TU RED
const char* ssid = "TU_WIFI";
const char* password = "TU_PASSWORD";

// COMPLETAR CON LA URL /exec DE TU APLICACIÓN DE GOOGLE APPS SCRIPT
const char* servidorGoogle =
  "TU_URL_DE_GOOGLE_APPS_SCRIPT";

WebServer servidor(80);

LiquidCrystal_I2C lcd(0x27, 16, 2);

#define ONE_WIRE_BUS 4

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensores(&oneWire);

float temperatura = 0;

const unsigned long intervaloRegistro = 10000;
unsigned long ultimoRegistro = 0;

const float limitePrealarma = 25.0;
const float limiteAlarma = 30.0;

enum EstadoTemperatura {
  NORMAL,
  PREALARMA,
  ALARMA
};

EstadoTemperatura estadoActual = NORMAL;
EstadoTemperatura estadoAnterior = NORMAL;

const char pagina[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<title>Monitor de Temperatura</title>
<style>
body{font-family:Arial;background:#f2f2f2;text-align:center;margin-top:60px;}
h1{color:#1565C0;}
p{font-size:24px;}
.temperatura{font-size:60px;font-weight:bold;color:#E53935;}
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

void paginaPrincipal() {
  String html = pagina;
  html.replace("%TEMPERATURA%", String(temperatura, 1) + " &deg;C");
  servidor.send(200, "text/html", html);
}

void enviarTemperatura() {
  servidor.send(200, "text/plain", String(temperatura, 1));
}

EstadoTemperatura determinarEstado(float temp) {
  if (temp < limitePrealarma) {
    return NORMAL;
  }
  if (temp <= limiteAlarma) {
    return PREALARMA;
  }
  return ALARMA;
}

void setup() {

  Serial.begin(115200);

  lcd.init();
  lcd.backlight();

  sensores.begin();

  lcd.setCursor(0, 0);
  lcd.print("Temperatura:");

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

  configTime(
    -3 * 3600,
    0,
    "pool.ntp.org",
    "time.nist.gov"
  );

  Serial.println("Sincronizando fecha y hora...");

  struct tm tiempo;

  while (!getLocalTime(&tiempo)) {
    Serial.print(".");
    delay(500);
  }

  Serial.println();
  Serial.println("Fecha y hora sincronizadas.");

  Serial.print("Fecha: ");
  Serial.printf(
    "%02d/%02d/%04d\n",
    tiempo.tm_mday,
    tiempo.tm_mon + 1,
    tiempo.tm_year + 1900
  );

  Serial.print("Hora: ");
  Serial.printf(
    "%02d:%02d:%02d\n",
    tiempo.tm_hour,
    tiempo.tm_min,
    tiempo.tm_sec
  );

  servidor.on("/", paginaPrincipal);
  servidor.on("/temperatura", enviarTemperatura);

  servidor.begin();

  Serial.println("Servidor Web iniciado");
}

void loop() {

  sensores.requestTemperatures();
  temperatura = sensores.getTempCByIndex(0);

  estadoActual = determinarEstado(temperatura);

  if (estadoActual != estadoAnterior) {

    Serial.print("CAMBIO DE ESTADO: ");

    if (estadoActual == NORMAL) {
      Serial.println("NORMAL");
    }
    else if (estadoActual == PREALARMA) {
      Serial.println("PREALARMA");
    }
    else if (estadoActual == ALARMA) {
      Serial.println("ALARMA");
    }

    estadoAnterior = estadoActual;
  }

  lcd.setCursor(0, 1);
  lcd.print("                ");
  lcd.setCursor(0, 1);
  lcd.print(temperatura, 1);
  lcd.print((char)223);
  lcd.print("C");

  struct tm tiempoActual;

  if (getLocalTime(&tiempoActual)) {

    char datos[50];

    snprintf(
      datos,
      sizeof(datos),
      "%02d/%02d/%04d | %02d:%02d:%02d | %.1f C",
      tiempoActual.tm_mday,
      tiempoActual.tm_mon + 1,
      tiempoActual.tm_year + 1900,
      tiempoActual.tm_hour,
      tiempoActual.tm_min,
      tiempoActual.tm_sec,
      temperatura
    );

    Serial.println(datos);

    if (millis() - ultimoRegistro >= intervaloRegistro) {

      ultimoRegistro = millis();

      if (WiFi.status() == WL_CONNECTED) {

        HTTPClient http;

        String textoEstado;

        if (estadoActual == NORMAL) {
          textoEstado = "NORMAL";
        }
        else if (estadoActual == PREALARMA) {
          textoEstado = "PREALARMA";
        }
        else {
          textoEstado = "ALARMA";
        }

        String url =
          String(servidorGoogle) +
          "?fecha=" +
          String(tiempoActual.tm_mday) +
          "/" +
          String(tiempoActual.tm_mon + 1) +
          "/" +
          String(tiempoActual.tm_year + 1900) +
          "&hora=" +
          String(tiempoActual.tm_hour) +
          ":" +
          String(tiempoActual.tm_min) +
          ":" +
          String(tiempoActual.tm_sec) +
          "&temperatura=" +
          String(temperatura, 1) +
          "&estado=" +
          textoEstado;

        http.begin(url);

        int codigoRespuesta = http.GET();

        Serial.print("Codigo HTTP: ");
        Serial.println(codigoRespuesta);

        http.end();
      }
    }

    servidor.handleClient();

    delay(1000);
  }
}
