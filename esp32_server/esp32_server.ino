#include <WiFi.h>
#include <ArduinoJson.h>
#include "FastAccelStepper.h"

// pinos do motor 1
#define dirPinStepper_1 22
#define enablePinStepper_1 23
#define stepPinStepper_1 2

// pinos do motor 2
#define dirPinStepper_2 13
#define enablePinStepper_2 12
#define stepPinStepper_2 25

// pinos do motor 2
#define dirPinStepper_3 18
#define enablePinStepper_3 19
#define stepPinStepper_3 20

// comandos motor 1
#define stopM1 0
#define accelerationM1 1
#define velocityM1 2
#define clockwiseM1 3
#define counterClockwiseM1 4
#define moveM1 5

// comandos motor 2
#define stopM2 6
#define accelerationM2 7
#define velocityM2 8
#define clockwiseM2 9
#define counterClockwiseM2 10
#define moveM2 11

// comandos motor 3
#define stopM3 12
#define accelerationM3 13
#define velocityM3 14
#define clockwiseM3 15
#define counterClockwiseM3 16
#define moveM3 17

// instâncias dos motores
FastAccelStepperEngine engine = FastAccelStepperEngine();
FastAccelStepper *stepper_1 = NULL;
FastAccelStepper *stepper_2 = NULL;
FastAccelStepper *stepper_3 = NULL;

// conexão com wifi
const char *ssid = "CARLA"; // Replace with your WiFi credentials
const char *password = "amorpropio";
const int serverPort = 8081; // Change this if needed

WiFiServer server(serverPort);

// Parse the values received from server to macros
const String valueTypes[] = {
    "stopM1", "accelerationM1", "velocityM1", "clockwiseM1", "counterClockwiseM1", "moveM1",
    "stopM2", "accelerationM2", "velocityM2", "clockwiseM2", "counterClockwiseM2", "moveM2",
    "stopM3", "accelerationM3", "velocityM3", "clockwiseM3", "counterClockwiseM3", "moveM3"};

const int numValueTypes = sizeof(valueTypes) / sizeof(valueTypes[0]);
int action = -1; // Initialize to invalid value

void setup()
{
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println("Connected to WiFi!");

  Serial.print("Starting TCP server on port: ");
  Serial.println(serverPort);

  server.begin();

  engine.init();

  stepper_1 = engine.stepperConnectToPin(stepPinStepper_1);
  stepper_2 = engine.stepperConnectToPin(stepPinStepper_2);
  stepper_3 = engine.stepperConnectToPin(stepPinStepper_3);

  if (stepper_1 && stepper_2)
  {

    stepper_1->setDirectionPin(dirPinStepper_1, 1);
    stepper_1->setEnablePin(enablePinStepper_1);
    stepper_1->setSpeedInUs(100);
    stepper_1->setAcceleration(20);

    stepper_2->setDirectionPin(dirPinStepper_2, 1);
    stepper_2->setEnablePin(enablePinStepper_2);
    stepper_2->setSpeedInUs(100);
    stepper_2->setAcceleration(20);

    stepper_3->setDirectionPin(dirPinStepper_3, 1);
    stepper_3->setEnablePin(enablePinStepper_3);
    stepper_3->setSpeedInUs(100);
    stepper_3->setAcceleration(20);

    stepper_1->enableOutputs();
    stepper_2->enableOutputs();
    stepper_3->enableOutputs();

    Serial.print("motors were enabled");
  }
}

void loop()
{

  WiFiClient client = server.available(); // Check for new connections

  Serial.println("Waiting for client to connect");
  // Print the IP address after successful WiFi connection
  IPAddress localIP = WiFi.localIP();
  Serial.print("ESP32 IP address: ");
  Serial.println(localIP);
  if (client)
  {
    Serial.println("Client connected!");
    // Define a buffer for storing the incoming JSON data
    char jsonBuffer[512]; // Adjust buffer size as needed
    int bytesRead = client.readBytes(jsonBuffer, sizeof(jsonBuffer));

    // Check if any data was read
    if (bytesRead > 0)
    {
      Serial.print("Received from client (");
      Serial.print(bytesRead);
      Serial.println(" bytes):");

      // Create a JsonDocument object to parse the JSON data
      DynamicJsonDocument doc(512); // Adjust document size as needed
      DeserializationError error = deserializeJson(doc, jsonBuffer, bytesRead);

      // Check for parsing errors
      if (error)
      {
        Serial.print("Error parsing JSON: ");
        Serial.println(error.c_str());
      }
      else
      {
        // Access data from the JSON object (replace with your property names)
        int value = doc["value"];
        String valueType = doc["valueType"]; // is converted to a integer, macro.
        String messageReceived = "Received action: " + valueType + ", value: " + value;
        Serial.println(messageReceived);

        // convert the string received to an integer, see macros.
        for (int i = 0; i < numValueTypes; i++)
        {
          if (valueType == valueTypes[i])
          {
            action = i;
            break;
          }
        }

        if (stepper_1 && stepper_2)
        {
          // Process the received value and valueType
          switch (action)
          {

          case stopM1:
            stepper_1->forceStopAndNewPosition(stepper_1->getCurrentPosition());
            client.println(messageReceived);
            break;

          case accelerationM1:
            stepper_1->setAcceleration(value);
            stepper_1->applySpeedAcceleration();
            client.println(messageReceived);
            break;

          case velocityM1:
            stepper_1->setSpeedInHz(value);
            client.println(messageReceived);
            break;

          case moveM1:
            stepper_1->moveTo(value);
            client.println(messageReceived);
            break;

          case clockwiseM1:
            // Motor code.
            stepper_1->setDirectionPin(dirPinStepper_1, 0); // check if 0 is indeed clockwise
            client.println(messageReceived);
            break;

          case counterClockwiseM1:
            // Motor code.
            stepper_1->setDirectionPin(dirPinStepper_1, 1);
            client.println(messageReceived);
            break;

          case stopM2:
            stepper_2->forceStopAndNewPosition(stepper_2->getCurrentPosition());
            client.println(messageReceived);
            break;

          case accelerationM2:
            stepper_2->setAcceleration(value);
            stepper_2->applySpeedAcceleration();
            client.println(messageReceived);
            break;

          case velocityM2:
            stepper_2->setSpeedInHz(value);
            client.println(messageReceived);
            break;

          case moveM2:
            stepper_2->moveTo(value);
            client.println(messageReceived);
            break;

          case clockwiseM2:
            // Motor code.
            stepper_2->setDirectionPin(dirPinStepper_2, 0); // check if 0 is indeed clockwise
            client.println(messageReceived);
            break;

          case counterClockwiseM2:
            // Motor code.
            stepper_2->setDirectionPin(dirPinStepper_2, 1);
            client.println(messageReceived);
            break;

          case stopM3:
            stepper_3->forceStopAndNewPosition(stepper_3->getCurrentPosition());
            client.println(messageReceived);
            break;

          case accelerationM3:
            stepper_3->setAcceleration(value);
            stepper_3->applySpeedAcceleration();
            client.println(messageReceived);
            break;

          case velocityM3:
            stepper_3->setSpeedInHz(value);
            client.println(messageReceived);
            break;

          case moveM3:
            stepper_3->moveTo(value);
            client.println(messageReceived);
            break;

          case clockwiseM3:
            // Motor code.
            stepper_3->setDirectionPin(dirPinStepper_3, 0); // check if 0 is indeed clockwise
            client.println(messageReceived);
            break;

          case counterClockwiseM3:
            // Motor code.
            stepper_3->setDirectionPin(dirPinStepper_3, 1);
            client.println(messageReceived);
            break;

          default:
            // Motor code.
            client.println(messageReceived);
            break;
          }
        }
        else
        {
          client.println("Error: Motors are not available to work, check them");
        }

        delay(500);
        client.stop(); // Close the connection
        Serial.println("Client disconnected.");
      }
    }
  }
}
