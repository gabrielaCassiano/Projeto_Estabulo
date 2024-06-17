#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>
#include "HX711.h"

// definindo login e senha wifi
const char* ssid = "gabi";
const char* password = "12345678";
WebServer server(80);
// fim def


// inicio definindo variaveis

#define LED_PIN 15
#define SERVO_PIN_UM 25
#define SERVO_PIN_DOIS 26
#define CARGA_DT 12
#define CARGA_SCKT 14

Servo servoMotor1;
Servo servoMotor2;
HX711 balanca;

float fator_calibracao = -6740;
float pesoAtual = 0;
bool food = false;

// fim definindo variaveis



//inicio config wifi/local server
void conectarWifi() {
   //ip
   
   //wifi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());


  // definindo rotas servidor
  server.on("/door/on", HTTP_GET, handleOpenDoor);
  server.on("/door/off", HTTP_GET, handleCloseDoor);
  server.on("/peso/food", HTTP_GET, handleSendWeight);
  server.on("/peso/loadFood", HTTP_GET, handleWeightRemote);
  server.begin();
  Serial.println("HTTP server started");

}


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


void handleRoot() {
  digitalWrite(LED_PIN, HIGH);
}

// inicio funçoes peso 
void zeraBalanca() {
  balanca.tare();
  Serial.println("Balança zerada");
}

void configuraBalanca() {
  balanca.begin(CARGA_DT, CARGA_SCKT);
  balanca.set_scale(fator_calibracao);
  delay(5000);
  zeraBalanca();
  Serial.println("Balança calibrada e tara definida!");
}

void handleWeightRemote() {

  //funcionando
  server.sendHeader("Access-Control-Allow-Origin", "*");
  servoMotor2.write(90);
  delay(1500);
  servoMotor2.write(0);
  server.send(200, "text/plain", "Comida Colocada!");
}

void handleFood() {
  if(food){
    servoMotor2.write(90);
    food = false;
  } else {
      //pesado
      servoMotor2.write(0);
  }
}

void handleWeight() {
  pesoAtual = balanca.get_units(5), 2;
  Serial.print("Peso: ");
  Serial.print(pesoAtual);
  
  if (pesoAtual < 5.00) {
    //pesado
    food = true;
    handleFood();
  } else {
    //leve
    food = false;
    handleFood();
  }
  Serial.println(" g");
  balanca.read();
  delay(1000);
  }

  // inicio função mandar peso 

  void handleSendWeight() {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200, "text/plain", String(pesoAtual));
  }

  // fim função mandar peso 

//fim funçoes peso


//inicio controle das portas
void handleOpenDoor() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  servoMotor1.write(90);
  digitalWrite(LED_PIN, HIGH); 
  server.send(200, "text/plain", "Porta Aberta");
}

void handleCloseDoor() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  servoMotor1.write(-30);
  digitalWrite(LED_PIN, LOW);
  server.send(200, "text/plain", "Porta Fechada");
}
//fim controle das portas

//inicio controle alimentador


