#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>
#include "HX711.h"


const char* ssid = "Gabizola";
const char* password = "alozibag27122002";

IPAddress local_IP(192, 168, 1, 184);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 0, 0);
WebServer server(80);


#define LED_PIN 23
#define SERVO_PIN_UM 25
#define SERVO_PIN_DOIS 26
#define CARGA_DT 12
#define CARGA_SCKT 14

Servo servoMotor;
HX711 balanca;

float fator_calibracao = -7050;


void setup() {
  Serial.begin(115200);

  // Setup WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("STA Failed to configure");
  }

  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());
  
  // Initialize Servo
  servoMotor.attach(SERVO_PIN_UM);
  servoMotor.attach(SERVO_PIN_DOIS);

  // Initialize LED
  pinMode(LED_PIN, OUTPUT);
  
  // Setup Web Server
  server.on("/open", HTTP_POST, handleOpenDoor);
  server.on("/close", HTTP_POST, handleCloseDoor);
  server.begin();
  Serial.println("HTTP server started");
  
  
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

void handleOpenDoor() {
  servoMotor.write(90); // Open position
  digitalWrite(LED_PIN, HIGH); // Turn on LED
//   server.send(200, "text/html", "Door Opened");
}

void handleCloseDoor() {
  servoMotor.write(0); // Close position
  digitalWrite(LED_PIN, LOW); // Turn off LED
//   server.send(200, "text/html", "Door Closed");
}

void handleWeight() {
  Serial.print("Peso: ");
  Serial.print(balanca.get_units(), 2);
  Serial.println(" kg");
  delay(500);

}
