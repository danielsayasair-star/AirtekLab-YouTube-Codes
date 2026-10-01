/*
   Sistema de Monitoreo de Temperatura WiFi
   Capítulo 1
   Display LCD 16x2 I2C con ESP32
*/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

//======================
// LCD
//======================

// Dirección I2C: 0x27
// Tamaño: 16 columnas x 2 filas
LiquidCrystal_I2C lcd(0x27, 16, 2);

//======================
// VARIABLES
//======================

// Por ahora la temperatura será simulada.
// En el próximo capítulo este valor será leído
// desde el sensor DS18B20.
float temperatura = 25.0;

void setup() {

  // Iniciamos el Monitor Serie.
  Serial.begin(115200);

  // Inicializamos el display.
  lcd.init();

  // Encendemos la retroiluminación.
  lcd.backlight();

  // Escribimos los textos fijos.
  lcd.setCursor(0, 0);
  lcd.print("Temperatura:");

}

void loop() {

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

  // Simulamos una pequeña variación para que
  // el display cambie constantemente.
  temperatura += 0.1;

  // Cuando llega a 30 °C vuelve a 25 °C.
  if (temperatura > 30.0) {

    temperatura = 25.0;

  }

  // Esperamos un segundo.
  delay(1000);

}
