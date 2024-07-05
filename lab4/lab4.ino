// Incluye las librerías necesarias (si son requeridas)
#include <SoftwareSerial.h>

// Configura los pines de comunicación Serial
#define RAK_SERIAL Serial1 // Define el puerto serial del Arduino conectado al RAK

void setup() {
  // Inicializa la comunicación serial con el PC para depuración
  Serial.begin(115200);
  while (!Serial) {
    ; // Espera por la conexión serial
  }
  Serial.println("Iniciando prueba de comunicación con el módulo RAK");

  // Inicializa la comunicación serial con el módulo RAK
  RAK_SERIAL.begin(115200);
  delay(1000);

  // Envía el comando AT+VER=? para verificar la versión
  sendCommand("AT+VER=?");

  Serial.println("Prueba completada");

  sendCommand("AT+NWM=0");
  sendCommand("AT+PRECV=0");
  sendCommand("AT+P2P=924000000:7:125:0:10:14");
  
}

void loop() {
  sendCommand("AT+PSEND=01");
  delay(3000);
}

void sendCommand(String command) {
  RAK_SERIAL.println(command);
  Serial.println("Comando enviado: " + command);
  delay(1000); // Espera un momento para asegurar que el comando se procese

  while (RAK_SERIAL.available()) {
    String response = RAK_SERIAL.readString();
    Serial.println("Respuesta del módulo: " + response);
  }
}