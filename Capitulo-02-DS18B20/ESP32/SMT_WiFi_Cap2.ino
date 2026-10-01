/*
   Sistema de Monitoreo de Temperatura WiFi
   Capítulo 1
   Display LCD 16x2 I2C con ESP32
*/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// NUEVO: Librerías para el sensor DS18B20
#include <OneWire.h>
#include <DallasTemperature.h>

//======================
// LCD
//======================

// Dirección I2C: 0x27
// Tamaño: 16 columnas x 2 filas
LiquidCrystal_I2C lcd(0x27, 16, 2);

//======================
// NUEVO: DS18B20
//======================

// NUEVO: Pin donde conectamos el sensor
#define ONE_WIRE_BUS 4

// NUEVO: Creamos el bus OneWire
OneWire oneWire(ONE_WIRE_BUS);

// NUEVO: Creamos el objeto que controlará el sensor
DallasTemperature sensores(&oneWire);


//======================
// VARIABLES
//======================

// NUEVO: La temperatura ahora será leída desde el sensor DS18B20.
float temperatura = 0;

void setup() {

  // Iniciamos el Monitor Serie.
  Serial.begin(115200);

  // Inicializamos el display.
  lcd.init();

  // Encendemos la retroiluminación.
  lcd.backlight();

  // NUEVO: Inicializamos el sensor DS18B20.
  sensores.begin();

  // Escribimos los textos fijos.
  lcd.setCursor(0, 0);
  lcd.print("Temperatura:");

}

void loop() {

  // NUEVO: Solicitamos una nueva medición.
  sensores.requestTemperatures();

  // NUEVO: Guardamos la temperatura medida.
  temperatura = sensores.getTempCByIndex(0);

  // Mostramos la temperatura.
  lcd.setCursor(0, 1);

  // Sobrescribimos toda la línea para evitar
  // que queden caracteres de una lectura anterior.
  lcd.print("                ");

  // Volvemos al inicio de la línea.
  lcd.setCursor(0, 1);

  // Mostramos el valor de la temperatura.
  lcd.print(temperatura, 1);

  // Símbolo de grados.
  lcd.print((char)223);

  // Grados Celsius.
  lcd.print("C");

  // Enviamos el dato al Monitor Serie.
  Serial.print("Temperatura: ");
  Serial.print(temperatura);
  Serial.println(" C");

  // Esperamos un segundo.
  delay(1000);

}
