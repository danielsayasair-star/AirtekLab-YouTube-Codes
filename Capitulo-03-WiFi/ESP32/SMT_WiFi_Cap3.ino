/*
   Sistema de Monitoreo de Temperatura WiFi
   Capítulo 3

   ESP32 + DS18B20 + LCD I2C + WiFi
*/

//======================
// LIBRERÍAS
//======================

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// NUEVO: Librería para utilizar el WiFi del ESP32.
#include <WiFi.h>


//======================
// WIFI
//======================

// NUEVO: Nombre de la red WiFi.
const char* ssid = "nombre de la red";

// NUEVO: Contraseña de la red WiFi.
const char* password = "contraseña";


//======================
// LCD
//======================

LiquidCrystal_I2C lcd(0x27, 16, 2);


//======================
// DS18B20
//======================

#define ONE_WIRE_BUS 4

OneWire oneWire(ONE_WIRE_BUS);

DallasTemperature sensores(&oneWire);


//======================
// VARIABLES
//======================

float temperatura = 0;


//======================
// SETUP
//======================

void setup() {

  // Iniciamos el Monitor Serie.
  Serial.begin(115200);

  // Inicializamos el display.
  lcd.init();

  // Encendemos la retroiluminación.
  lcd.backlight();

  // Inicializamos el sensor.
  sensores.begin();

  // Escribimos los textos fijos.
  lcd.setCursor(0, 0);
  lcd.print("Temperatura:");

  // NUEVO: Iniciamos la conexión WiFi.
  WiFi.begin(ssid, password);

  // NUEVO: Esperamos hasta que el ESP32 logre conectarse.
  while (WiFi.status() != WL_CONNECTED) {

    delay(500);
    Serial.print(".");

  }

  // NUEVO: Informamos que la conexión fue exitosa.
  Serial.println();
  Serial.println("WiFi conectado.");

  // NUEVO: Mostramos la dirección IP asignada.
  Serial.print("Direccion IP: ");
  Serial.println(WiFi.localIP());

}


//======================
// LOOP
//======================

void loop() {

  // Solicitamos una nueva medición.
  sensores.requestTemperatures();

  // Guardamos la temperatura medida.
  temperatura = sensores.getTempCByIndex(0);

  // Mostramos la temperatura.
  lcd.setCursor(0, 1);

  // Borramos la línea para evitar
  // que queden caracteres anteriores.
  lcd.print("                ");

  // Volvemos al inicio.
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

  delay(1000);

}