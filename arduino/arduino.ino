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

float fator_calibracao = -7000*1000;
float pesoAtual = 0;
bool food = false;


void setup() {
  Serial.begin(115200);

  //setando pinos
  servoMotor1.attach(SERVO_PIN_UM);
  servoMotor2.attach(SERVO_PIN_DOIS);
  pinMode(LED_PIN, OUTPUT);
  
  //chamando funçoes do estabulo
  conectarWifi();
  configuraBalanca();

}

void loop() {
  //esperando requisiçoes
  server.handleClient();

  //trabalhando o peso
  handleWeight();
}

void configuraBalanca() {
  balanca.begin(CARGA_DT, CARGA_SCKT);
  balanca.set_scale(fator_calibracao);
  balanca.tare();
  Serial.println("Balança calibrada e tara definida!");
}

void handleRoot() {
  digitalWrite(LED_PIN, HIGH);
}


//inicio controle das portas
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
//fim controle das portas

//inicio controle alimentador
void handleFood() {
  if(food){
    servoMotor2.write(88);
    food = false;
  } else {
      servoMotor2.write(-2);
  }
}

void handleWeight() {
  pesoAtual = balanca.get_units(), 2;
  Serial.print("Peso: ");
  Serial.print(balanca.get_units(), 2);
  if (pesoAtual > 0.05) {
    //pesado
    food = true;
    handleFood();
  } else {
    //leve
    food = false;
    handleFood();
  }
  Serial.println(" kg");
  balanca.read();
  delay(500);
}
//fim controle alimentador


//inicio config wifi/local server
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
