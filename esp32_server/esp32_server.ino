#include <WiFi.h>

const char* ssid = "CARLA"; // Replace with your WiFi credentials
const char* password = "amorpropio";
const int serverPort = 8081; // Change this if needed

WiFiServer server(serverPort);

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("Connected to WiFi!");

  Serial.print("Starting TCP server on port: ");
  Serial.println(serverPort);
 
  server.begin();
}

void loop() {
  WiFiClient client = server.available(); // Check for new connections

  Serial.println("Waiting for client to connect");
  // Print the IP address after successful WiFi connection
  IPAddress localIP = WiFi.localIP();
  Serial.print("ESP32 IP address: ");
  Serial.println(localIP);
  if (client) {
    Serial.println("Client connected!");

    while (client.connected()) {
      Serial.print("Inside the while loop");
      String line = client.readStringUntil('\n'); // Read data until newline

      if (line.length() > 0) {
        Serial.print("Received from client: ");
        Serial.println(line);

        // Process the received data from the client (e.g., convert to number, etc.)

        // Send a response back to the client
        client.println("Message received!");
        delay(500);
        client.stop(); // Close the connection
        Serial.println("Client disconnected.");
      }
      
    }
   
  }
}
