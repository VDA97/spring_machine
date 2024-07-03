#include <WiFi.h>
#include <ArduinoJson.h>

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
          // Define a buffer for storing the incoming JSON data
      char jsonBuffer[512]; // Adjust buffer size as needed
      int bytesRead = client.readBytes(jsonBuffer, sizeof(jsonBuffer));

      // Check if any data was read
      if (bytesRead > 0) {
        Serial.print("Received from client (");
        Serial.print(bytesRead);
        Serial.println(" bytes):");

        // Create a JsonDocument object to parse the JSON data
        DynamicJsonDocument doc(512);; // Adjust document size as needed
        DeserializationError error = deserializeJson(doc, jsonBuffer, bytesRead);

        // Check for parsing errors
        if (error) {
          Serial.print("Error parsing JSON: ");
          Serial.println(error.c_str());
        } else {
          // Access data from the JSON object (replace with your property names)
          int value = doc["value"];
          String valueType = doc["valueType"];

          Serial.print("Received value: ");
          Serial.println(value);
          Serial.print("Received value type: ");
          Serial.println(valueType);

          // Process the received value and valueType
          // (e.g., control motors, update states)

          // Send a response (optional)
          client.println("Message received!");
          delay(500);
          client.stop(); // Close the connection
          Serial.println("Client disconnected.");
        }
      }

    

  /*  while (client.connected()) {
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
      
    }*/
   
  }
}
