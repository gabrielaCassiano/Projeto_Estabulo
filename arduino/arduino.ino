#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>
#include "HX711.h"


const char* ssid = "gabi";
const char* password = "12345678";
WebServer server(80);


#define LED_PIN 15
#define SERVO_PIN_UM 25
#define SERVO_PIN_DOIS 26
#define CARGA_DT 12
#define CARGA_SCKT 14

Servo servoMotor1;
Servo servoMotor2;
HX711 balanca;

float fator_calibracao = -37000;


void setup() {
  Serial.begin(115200);

  conectarWifi();

  
  // Initialize Servo
  servoMotor1.attach(SERVO_PIN_UM);
  servoMotor2.attach(SERVO_PIN_DOIS);

  // Initialize LED
  pinMode(LED_PIN, OUTPUT);
  
  // Setup Balança
  balanca.begin(CARGA_DT, CARGA_SCKT);
  balanca.set_scale(fator_calibracao);
  balanca.tare();
  Serial.println("Balança calibrada e tara definida!");
}

void loop() {
  server.handleClient();
  handleWeight();
  
}

void handleRoot() {
  digitalWrite(LED_PIN, HIGH);
}

void handleOpenDoor() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  servoMotor1.write(88);
  digitalWrite(LED_PIN, HIGH); 
  server.send(200, "text/plain", "Porta Aberta");
}

void handleCloseDoor() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  servoMotor1.write(-2);
  digitalWrite(LED_PIN, LOW);
  server.send(200, "text/plain", "Porta Fechada");
}

void conectarWifi() {
   // Setup WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());



  server.on("/door/on", HTTP_GET, handleOpenDoor);
  server.on("/door/off", HTTP_GET, handleCloseDoor);
  server.begin();
  Serial.println("HTTP server started");

}

void handleWeight() {
  Serial.print("Peso: ");
  Serial.print(balanca.get_units(), 2);
  Serial.println(" kg");
  delay(5000);

}
