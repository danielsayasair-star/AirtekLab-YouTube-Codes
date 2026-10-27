# Clase 02 — Programación con millis() en ESP32

En esta clase vas a aprender a utilizar `millis()` para temporizar tareas sin detener el programa con `delay()`, mediante cinco ejercicios prácticos.

📺 **Ver el video en YouTube:** https://youtu.be/CVT6r8H1FCE

## Hardware y software

- Placa: ESP32 Dev Module
- Entorno: Arduino IDE
- LED externo con resistencia limitadora: GPIO 25
- Sensor DHT22: DATA en GPIO 4, VCC a 3V3 y GND a GND
- Pulsador para el ejercicio 5: GPIO 18 a GND, configurado con `INPUT_PULLUP`
- Monitor Serie: 115200 baudios cuando corresponda

### Biblioteca requerida

Los ejercicios 3, 4 y 5 requieren **DHT sensor library** de Adafruit. Instalá la biblioteca desde el gestor de bibliotecas del Arduino IDE.

## Ejercicios

1. **LED sin `delay()`:** alterna el estado del LED cada 1000 ms.
2. **Dos tareas independientes:** LED cada 1000 ms y mensaje por Monitor Serie cada 2500 ms.
3. **Lectura periódica del DHT22:** muestra temperatura y humedad cada 2000 ms.
4. **Salida temporizada:** enviar `1` por Monitor Serie activa el LED durante 3 segundos; enviar `0` lo apaga inmediatamente. La lectura del DHT22 continúa.
5. **Detección de inactividad:** cada pulsación reinicia el contador. Después de 10 segundos sin pulsaciones se enciende el LED de aviso. Una nueva pulsación apaga el aviso y reinicia el contador. La lectura del DHT22 continúa.

## Archivos

- [Ejercicio 1 — LED sin delay()](./01_LED_Sin_Delay/01_LED_Sin_Delay.ino)
- [Ejercicio 2 — Dos tareas independientes](./02_Dos_Tareas_Independientes/02_Dos_Tareas_Independientes.ino)
- [Ejercicio 3 — Lectura periódica del DHT22](./03_Lectura_Periodica_DHT22/03_Lectura_Periodica_DHT22.ino)
- [Ejercicio 4 — Salida temporizada](./04_Salida_Temporizada/04_Salida_Temporizada.ino)
- [Ejercicio 5 — Detección de inactividad](./05_Deteccion_Inactividad/05_Deteccion_Inactividad.ino)

## Antes de empezar

- Revisá las conexiones y utilizá una resistencia limitadora adecuada para el LED.
- Para los ejercicios que utilizan el DHT22, verificá que la biblioteca esté instalada y que el sensor esté conectado correctamente.
- En el ejercicio 5, el pulsador se conecta entre GPIO 18 y GND; el programa utiliza `INPUT_PULLUP`.
- Abrí el Monitor Serie a 115200 baudios en los ejercicios que lo utilizan.

## Concepto clave

`millis()` devuelve los milisegundos transcurridos desde que arrancó el programa. La comparación se realiza restando el tiempo anterior al actual:

```cpp
if (tiempoActual - tiempoAnterior >= INTERVALO) {
  tiempoAnterior = tiempoActual;
  // Ejecutar la tarea
}
```

Este patrón permite programar acciones periódicas sin esperar con `delay()`. No significa que cualquier función de biblioteca sea completamente no bloqueante; por ejemplo, una lectura del DHT22 puede ocupar brevemente el procesador.

Los ejercicios de esta carpeta corresponden a la clase práctica de AirtekLab sobre `millis()`. Consultá el video para ver el montaje y las explicaciones paso a paso.
