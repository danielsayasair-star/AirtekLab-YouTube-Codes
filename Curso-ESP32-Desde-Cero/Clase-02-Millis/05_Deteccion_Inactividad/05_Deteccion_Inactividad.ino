// AirtekLab - Programacion con millis() y ESP32
// Ejercicio 05: detectar inactividad y mostrar el tiempo transcurrido
// Placa: ESP32 Dev Module
// LED de aviso: GPIO 25 con resistencia limitadora
// Pulsador: GPIO 18 a GND, usando INPUT_PULLUP
// DHT22: DATA en GPIO 4
// Monitor Serie: 115200 baudios
// Si no se presiona el boton durante 10 segundos, se enciende el LED.
// Cada pulsacion reinicia el contador de inactividad.

#include <DHT.h>

const int PIN_LED = 25;
const int PIN_PULSADOR = 18;
const int PIN_DHT = 4;

const unsigned long INTERVALO_LECTURA = 2000;
const unsigned long TIEMPO_INACTIVIDAD = 10000;
const unsigned long TIEMPO_ANTIRREBOTE = 40;
const unsigned long INTERVALO_REPORTE = 1000;

DHT sensorDHT(PIN_DHT, DHT22);

unsigned long tiempoLecturaAnterior = 0;
unsigned long momentoUltimaActividad = 0;
unsigned long momentoUltimoCambio = 0;
unsigned long tiempoReporteAnterior = 0;

int lecturaAnterior = HIGH;
int estadoPulsadorEstable = HIGH;

bool avisoInactividad = false;

void setup() {
  Serial.begin(115200);

  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_PULSADOR, INPUT_PULLUP);

  digitalWrite(PIN_LED, LOW);
  sensorDHT.begin();

  momentoUltimaActividad = millis();
  tiempoReporteAnterior = momentoUltimaActividad;

  Serial.println("Ejercicio 05 - Deteccion de inactividad");
  Serial.println("Presiona el boton para reiniciar el contador.");
  Serial.println("El LED se enciende al llegar a 10 segundos sin actividad.");
}

void loop() {
  unsigned long tiempoActual = millis();

  // Tarea 1: leer temperatura y humedad cada 2 segundos.
  if (tiempoActual - tiempoLecturaAnterior >= INTERVALO_LECTURA) {
    tiempoLecturaAnterior = tiempoActual;

    float humedad = sensorDHT.readHumidity();
    float temperatura = sensorDHT.readTemperature();

    if (isnan(humedad) || isnan(temperatura)) {
      Serial.println("Error: no se pudo leer el DHT22");
    } else {
      Serial.print("Temperatura: ");
      Serial.print(temperatura, 1);
      Serial.print(" C | Humedad: ");
      Serial.print(humedad, 1);
      Serial.println(" %");
    }
  }

  // Tarea 2: detectar una pulsacion estable con antirrebote.
  int lecturaActual = digitalRead(PIN_PULSADOR);

  if (lecturaActual != lecturaAnterior) {
    lecturaAnterior = lecturaActual;
    momentoUltimoCambio = tiempoActual;
  }

  if (tiempoActual - momentoUltimoCambio >= TIEMPO_ANTIRREBOTE) {
    if (lecturaActual != estadoPulsadorEstable) {
      estadoPulsadorEstable = lecturaActual;

      // La actividad se registra al presionar, cuando la entrada pasa a LOW.
      if (estadoPulsadorEstable == LOW) {
        momentoUltimaActividad = tiempoActual;
        tiempoReporteAnterior = tiempoActual;

        Serial.println("Actividad detectada: contador reiniciado a 0 s");

        if (avisoInactividad) {
          avisoInactividad = false;
          digitalWrite(PIN_LED, LOW);
          Serial.println("Aviso apagado");
        }
      }
    }
  }

  // Tarea 3: mostrar cada segundo cuanto tiempo lleva sin actividad.
  if (tiempoActual - tiempoReporteAnterior >= INTERVALO_REPORTE) {
    tiempoReporteAnterior = tiempoActual;

    unsigned long segundosInactivo =
        (tiempoActual - momentoUltimaActividad) / 1000;

    Serial.print("Tiempo sin actividad: ");
    Serial.print(segundosInactivo);
    Serial.println(" s");
  }

  // Tarea 4: activar el aviso cuando se cumple el tiempo limite.
  if (!avisoInactividad &&
      (tiempoActual - momentoUltimaActividad >= TIEMPO_INACTIVIDAD)) {
    avisoInactividad = true;
    digitalWrite(PIN_LED, HIGH);
    Serial.println("AVISO: se alcanzaron 10 segundos sin actividad");
  }
}
