#include <WiFi.h>
#include <WebServer.h>
#include <Servo.h>

const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";
WebServer server(80);
Servo servoMotor;

const int ledPin = 2; // Pin connected to the LED
const int servoPin = 18; // Pin connected to the servo

void setup() {
  Serial.begin(115200);

  // Setup WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  
  // Initialize Servo
  servoMotor.attach(servoPin);

  // Initialize LED
  pinMode(ledPin, OUTPUT);
  
  // Setup Web Server
  server.on("/", HTTP_GET, handleRoot);
  server.on("/open", HTTP_GET, handleOpenDoor);
  server.on("/close", HTTP_GET, handleCloseDoor);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}

void handleRoot() {
  server.send(200, "text/html", "<h1>ESP32 Door Control</h1><button onclick=\"sendCommand('open')\">Open Door</button><button onclick=\"sendCommand('close')\">Close Door</button><script>function sendCommand(command) { fetch('/' + command); }</script>");
}

void handleOpenDoor() {
  servoMotor.write(90); // Open position
  digitalWrite(ledPin, HIGH); // Turn on LED
  server.send(200, "text/html", "Door Opened");
}

void handleCloseDoor() {
  servoMotor.write(0); // Close position
  digitalWrite(ledPin, LOW); // Turn off LED
  server.send(200, "text/html", "Door Closed");
}
