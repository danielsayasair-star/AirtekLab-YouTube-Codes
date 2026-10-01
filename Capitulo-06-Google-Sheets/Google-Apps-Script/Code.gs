function doGet(e) {

  const hoja = SpreadsheetApp.getActiveSpreadsheet().getSheets()[0];

  const fecha = e.parameter.fecha;
  const hora = e.parameter.hora;
  const temperatura = Number(e.parameter.temperatura);

  hoja.appendRow([fecha, hora, temperatura]);

  return ContentService
    .createTextOutput("OK");
}