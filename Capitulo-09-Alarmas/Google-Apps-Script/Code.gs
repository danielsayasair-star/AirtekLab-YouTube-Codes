function doGet(e) {

  const hoja = SpreadsheetApp.getActiveSpreadsheet().getSheets()[0];

  const fecha = e.parameter.fecha;
  const hora = e.parameter.hora;
  const temperatura = Number(e.parameter.temperatura);
  const estado = e.parameter.estado;

  // Guardar el registro en Google Sheets
  hoja.appendRow([
    fecha,
    hora,
    temperatura,
    estado
  ]);

  // Recuperar el último estado registrado
  const propiedades = PropertiesService.getScriptProperties();
  const estadoAnterior = propiedades.getProperty("ESTADO_ANTERIOR");

  // Detectar cambio de estado
  if (estadoAnterior !== estado) {

    // Enviar email solamente al entrar en PREALARMA o ALARMA
    if (estado === "PREALARMA" || estado === "ALARMA") {

      // COMPLETAR CON EL EMAIL DONDE QUIERES RECIBIR LAS ALERTAS
      const destinatario = "TU_EMAIL@EJEMPLO.COM";

      const asunto = "Airtek Lab - " + estado;

      const mensaje =
        "ALERTA DE TEMPERATURA\n\n" +
        "Estado: " + estado + "\n" +
        "Temperatura: " + temperatura + " °C\n" +
        "Fecha: " + fecha + "\n" +
        "Hora: " + hora + "\n\n" +
        "Sistema de Monitoreo de Temperatura\n" +
        "Airtek Lab";

      MailApp.sendEmail(
        destinatario,
        asunto,
        mensaje
      );
    }

    // Guardar el nuevo estado
    propiedades.setProperty("ESTADO_ANTERIOR", estado);
  }

  return ContentService
    .createTextOutput("OK");
}

function probarEmail() {

  // COMPLETAR CON EL EMAIL DONDE QUIERES RECIBIR LA PRUEBA
  const destinatario = "TU_EMAIL@EJEMPLO.COM";

  MailApp.sendEmail(
    destinatario,
    "Airtek Lab - Prueba de correo",
    "Este es un correo de prueba del sistema de monitoreo de temperatura de Airtek Lab."
  );
}
