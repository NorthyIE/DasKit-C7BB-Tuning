// Import required libraries
#include <WiFi.h>
#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <mDNS.h>
#include <Update.h>

// Define WiFi credentials for the Access Point
const char* ssid = "Moscow Plus";
const char* password = "Moscow1234";

// IP configuration for the Access Point
IPAddress local_ip(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
AsyncWebServer server(80);

// OTA (Over-the-Air) Update settings
const char* serverIndex = "<form method='POST' action='/update' enctype='multipart/form-data'><input type='file' name='update'><input type='submit' value='Update'></form>";
bool restartRequired = false;  // Flag to trigger a restart after an update

// Global variable to store the current tuning state
String tuningState = "AUS"; // Default to "AUS" (off) on boot

// Tuning-specific variables for serial communication
const byte numBytes = 32;
byte receivedBytes1[numBytes];
byte numReceived1 = 0;
byte receivedBytes2[numBytes];
byte numReceived2 = 0;
boolean newData1 = false;
boolean newData2 = false;
byte sendBytes[numBytes];

// Handler for 404 Not Found errors
void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Nothing here..");
}

void setup() {
  // Initialize serial communication for debugging
  Serial.begin(115200);
  // Initialize serial ports for communication with display and controller
  Serial1.begin(9600, SERIAL_8N1, 12, 13);
  Serial2.begin(9600, SERIAL_8N1, 14, 15);

  // Initialize SPIFFS (SPI Flash File System)
  if (!SPIFFS.begin()) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // Configure and start the WiFi Access Point
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  server.begin();

  // Print a startup message to the serial console
  Serial.println("-= NCM Moscow Plus Controller ready =-");
  Serial.println("  -= WiFi-AP & HTTP server started =-");

  // Define web server routes and handlers
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    if (tuningState == "AN") {
      request->send(SPIFFS, "/tuningan.html", "text/html");
    }
    else {
      request->send(SPIFFS, "/tuningaus.html", "text/html");
    }
  });

  server.on("/tuningan", HTTP_GET, [](AsyncWebServerRequest * request) {
    tuningState = "AN";
    Serial.println("Tuning enabled");
    request->redirect("/");
  });

  server.on("/tuningaus", HTTP_GET, [](AsyncWebServerRequest * request) {
    tuningState = "AUS";
    Serial.println("Tuning disabled");
    request->redirect("/");
  });

  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/style.css", "text/css");
  });

  // Handler for OTA firmware updates
  server.on("/update", HTTP_POST, [](AsyncWebServerRequest * request) {
    AsyncWebServerResponse *response = request->beginResponse(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
    response->addHeader("Connection", "close");
    response->addHeader("Access-Control-Allow-Origin", "*");
    restartRequired = true;
    request->send(response);
  }, [](AsyncWebServerRequest * request, String filename, size_t index, uint8_t *data, size_t len, bool final) {
    if (!index) {
      Serial.printf("UploadStart: %s\n", filename.c_str());
      Serial.setDebugOutput(true);
      uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
      if (!Update.begin(maxSketchSpace)) {
        Update.printError(Serial);
      }
    }
    if (Update.write(data, len) != len) {
      Update.printError(Serial);
    }
    if (final) {
      if (Update.end(true)) {
        Serial.printf("Update Success: %u B\nRebooting...\n", index + len);
      } else {
        Update.printError(Serial);
      }
      Serial.setDebugOutput(false);
    }
  });

  server.onNotFound(notFound);
}

// Function to receive bytes from Serial1 with start and end markers
void recvBytesWithStartEndMarkers1() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    byte startMarker = 0x3A;
    byte endMarker1 = 0x0D;
    byte endMarker2 = 0x0A;
    byte rb;
    unsigned long startTime = millis();
    unsigned long timeout = 50; // 50ms timeout

    while (millis() - startTime < timeout && newData1 == false) {
        if (Serial1.available() > 0) {
            rb = Serial1.read();

            if (recvInProgress == true) {
                if ((rb != endMarker2) || (receivedBytes1[ndx - 1] != endMarker1)) {
                    receivedBytes1[ndx] = rb;
                    ndx++;
                    if (ndx >= numBytes) {
                        ndx = numBytes - 1;
                    }
                } else {
                    receivedBytes1[ndx - 1] = '\0'; // Null-terminate the string
                    recvInProgress = false;
                    numReceived1 = ndx - 1;
                    ndx = 0;
                    newData1 = true;
                }
            } else if (rb == startMarker) {
                recvInProgress = true;
            }
        }
    }
    if (recvInProgress && (millis() - startTime >= timeout)) {
        Serial.println("Serial1 timeout");
        recvInProgress = false;
        ndx = 0;
    }
}

// Function to receive bytes from Serial2 with start and end markers
void recvBytesWithStartEndMarkers2() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    byte startMarker = 0x3A;
    byte endMarker1 = 0x0D;
    byte endMarker2 = 0x0A;
    byte rb;
    unsigned long startTime = millis();
    unsigned long timeout = 50; // 50ms timeout

    while (millis() - startTime < timeout && newData2 == false) {
        if (Serial2.available() > 0) {
            rb = Serial2.read();

            if (recvInProgress == true) {
                if ((rb != endMarker2) || (receivedBytes2[ndx - 1] != endMarker1)) {
                    receivedBytes2[ndx] = rb;
                    ndx++;
                    if (ndx >= numBytes) {
                        ndx = numBytes - 1;
                    }
                } else {
                    receivedBytes2[ndx - 1] = '\0'; // Null-terminate the string
                    recvInProgress = false;
                    numReceived2 = ndx - 1;
                    ndx = 0;
                    newData2 = true;
                }
            } else if (rb == startMarker) {
                recvInProgress = true;
            }
        }
    }
    if (recvInProgress && (millis() - startTime >= timeout)) {
        Serial.println("Serial2 timeout");
        recvInProgress = false;
        ndx = 0;
    }
}

// Function to modify the received data for tuning
void modNewData() {
  sendBytes[0] = 0x3A;
  for (byte n = 0; n < numReceived1; n++) {
    sendBytes[n + 1] = receivedBytes1[n];
  }
  sendBytes[10] = 0x0D;
  sendBytes[11] = 0x0A;

  if (tuningState == "AN") {
    if (sendBytes[3] == (0x42)) {
      sendBytes[2] = (0x0B);
      sendBytes[3] = (0x46);
      sendBytes[4] = (0x24);
    }
  }
}

// Function to calculate the checksum of the data
void Checksumm() {
  sendBytes[8] = 0;
  for (byte n = 1; n < 8; n++) {
    sendBytes[8] = sendBytes[8] + sendBytes[n];
  }
}

// Function to process and display new data
void showNewData() {
  if (newData1 == true) {
    modNewData();
    Checksumm();
    Serial.print("received: ");
    for (byte n = 0; n < numReceived1; n++) {
      Serial.print(receivedBytes1[n], HEX);
      Serial.print(' ');
    }
    Serial.println();

    Serial.print("mod: ");
    for (byte n = 0; n < 12; n++) {
      Serial.print(sendBytes[n], HEX);
      Serial.print(' ');
    }
    Serial.println();
    Serial1.write(sendBytes, 12);
    newData1 = false;
  }

  if (newData2 == true) {
    Serial.print("receivedMotorC: ");
    for (byte n = 0; n < numReceived2; n++) {
      Serial.print(receivedBytes2[n], HEX);
      Serial.print(' ');
    }
    Serial.println();
    newData2 = false;
  }
}

// Main tuning loop
void tuning() {
  recvBytesWithStartEndMarkers1();
  showNewData();
  recvBytesWithStartEndMarkers2();
}

// OTA Update loop
void ota() {
  if (restartRequired) {
    Serial.printf("Restarting ESP\n\r");
    restartRequired = false;
    ESP.restart();
  }
}

// Main loop
void loop() {
  tuning();
  ota();
}
