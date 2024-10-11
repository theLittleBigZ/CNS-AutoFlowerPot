#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// Replace with your network credentials
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

// Define pin connections
const int SensorPin = A0;   // Analog pin for soil moisture sensor
const int relayPin = D0;    // Digital pin for relay control

// Initialize variables
int soilMoistureValue = 0;
int soilMoisturePercent = 0;
int desiredMoisturePercent = 50; // Default desired moisture percentage

// Initialize web server on port 80
ESP8266WebServer server(80);

// Function declarations
void handleRoot();
void handleSetMoisture();
void handlePumpBurst();
void activatePump(int duration);
String generateHTML();

void setup() {
  // Initialize serial communication for debugging
  Serial.begin(115200);
  
  // Initialize relay pin
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH); // Assuming HIGH is OFF

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());

  // Define web server routes
  server.on("/", handleRoot);
  server.on("/setMoisture", HTTP_POST, handleSetMoisture);
  server.on("/pumpBurst", HTTP_POST, handlePumpBurst);
  
  // Start the server
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  // Handle client requests
  server.handleClient();

  // Read soil moisture sensor
  soilMoistureValue = analogRead(SensorPin);
  
  // Convert sensor value to percentage (Assuming 0 - 1023 maps to 0% - 100%)
  soilMoisturePercent = map(soilMoistureValue, 0, 1023, 100, 0); // Inverted if needed
  
  Serial.print("Soil Moisture: ");
  Serial.print(soilMoisturePercent);
  Serial.println("%");
  
  // Automatic pump control
  if (soilMoisturePercent < desiredMoisturePercent) {
    Serial.println("Soil is dry. Activating pump.");
    activatePump(3000); // Activate pump for 3 seconds
  }
  
  delay(5000); // Wait for 5 seconds before next reading
}

void handleRoot() {
  String html = generateHTML();
  server.send(200, "text/html", html);
}

void handleSetMoisture() {
  if (server.hasArg("desiredMoisture")) {
    String moistureStr = server.arg("desiredMoisture");
    desiredMoisturePercent = moistureStr.toInt();
    Serial.print("Desired Moisture Set To: ");
    Serial.print(desiredMoisturePercent);
    Serial.println("%");
  }
  // Redirect back to root
  server.sendHeader("Location", "/");
  server.send(303);
}

void handlePumpBurst() {
  Serial.println("Manual pump burst activated.");
  activatePump(3000); // Activate pump for 3 seconds
  // Redirect back to root
  server.sendHeader("Location", "/");
  server.send(303);
}

void activatePump(int duration) {
  digitalWrite(relayPin, LOW); // Assuming LOW activates the pump
  delay(duration);
  digitalWrite(relayPin, HIGH); // Turn off the pump
}

String generateHTML() {
  String html = "<!DOCTYPE html><html>";
  html += "<head><meta name='viewport' content='width=device-width, initial-scale=1'>";
  html += "<title>Soil Moisture Controller</title>";
  html += "<style>body { font-family: Arial; text-align: center; padding: 50px; } ";
  html += "input[type=number] { padding: 10px; margin: 10px; width: 80px; } ";
  html += "input[type=submit], button { padding: 10px 20px; margin: 10px; } ";
  html += "</style></head><body>";
  html += "<h1>Soil Moisture Controller</h1>";
  html += "<p>Current Soil Moisture: <strong>" + String(soilMoisturePercent) + "%</strong></p>";
  
  html += "<form action='/setMoisture' method='POST'>";
  html += "<label for='desiredMoisture'>Set Desired Moisture (%): </label>";
  html += "<input type='number' id='desiredMoisture' name='desiredMoisture' min='0' max='100' value='" + String(desiredMoisturePercent) + "'>";
  html += "<input type='submit' value='Set'>";
  html += "</form>";
  
  html += "<form action='/pumpBurst' method='POST'>";
  html += "<button type='submit'>Activate Pump (3 sec)</button>";
  html += "</form>";
  
  html += "</body></html>";
  return html;
}
