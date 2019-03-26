// Import libraries
#include <WiFi.h>
#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>

// WiFi-AP SSID & Password
const char* ssid = "Moscow Plus";
const char* password = "Moscow1234";

// IP-Configuration
IPAddress local_ip(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
AsyncWebServer server(80);

// Stores the current states, disables tuning on boot
String tuningState = "AUS";

// Tuning-specific
const byte numBytes = 32;
byte receivedBytes1[numBytes];
byte numReceived1 = 0;
byte receivedBytes2[numBytes];
byte numReceived2 = 0;
boolean newData1 = false;
boolean newData2 = false;
byte sendBytes[numBytes];

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Nothing here..");
}

void setup() {
  // Serial configuration
  Serial.begin(115200);                                    // USB-Serial console
  Serial1.begin(9600, SERIAL_8N1, 12, 13);                 // Additional consoles for display and controller communication
  Serial2.begin(9600, SERIAL_8N1, 14, 15);

  // Starting WiFi-AP
  if (!SPIFFS.begin()) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  server.begin();

  // Show on console when booted
  Serial.println("-= NCM Moscow Plus Controller ready =-");
  Serial.println("  -= WiFi-AP & HTTP server started =-");

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
  server.onNotFound(notFound);

  server.begin();
}

void recvBytesWithStartEndMarkers1() {
  static boolean recvInProgress = false;
  static byte ndx = 0;
  byte startMarker = 0x3A;             //muss mit 0x3A beginnen
  byte endMarker1 = 0x0D;
  byte endMarker2 = 0x0A;              // und mit CR LF enden
  byte rb;

  while (Serial1.available() > 0 && newData1 == false) {
    rb = Serial1.read();

    if (recvInProgress == true) {
      if ((rb != endMarker2) || (receivedBytes1[ndx - 1] != endMarker1)) {
        receivedBytes1[ndx] = rb;
        ndx++;

        if (ndx >= numBytes) {
          ndx = numBytes - 1;
        }
      }
      else {
        receivedBytes1[ndx - 1] = '\0'; // terminate the string
        recvInProgress = false;
        numReceived1 = ndx - 1; // save the number for use when printing
        ndx = 0;
        newData1 = true;

      }
    }

    else if (rb == startMarker) {
      recvInProgress = true;

    }
  }
}

void recvBytesWithStartEndMarkers2() {
  static boolean recvInProgress = false;
  static byte ndx = 0;
  byte startMarker = 0x3A;             //muss mit 0x3A beginnen
  byte endMarker1 = 0x0D;
  byte endMarker2 = 0x0A;              // und mit CR LF enden
  byte rb;



  while (Serial2.available() > 0 && newData2 == false) {
    rb = Serial2.read();

    if (recvInProgress == true) {
      if ((rb != endMarker2) || (receivedBytes2[ndx - 1] != endMarker1)) {
        receivedBytes2[ndx] = rb;
        ndx++;

        if (ndx >= numBytes) {
          ndx = numBytes - 1;
        }
      }
      else {
        receivedBytes2[ndx - 1] = '\0';                     // terminate the string
        recvInProgress = false;
        numReceived2 = ndx - 1;                             // save the number for use when printing
        ndx = 0;
        newData2 = true;

      }
    }

    else if (rb == startMarker) {
      recvInProgress = true;

    }
  }
}

void modNewData() {
  sendBytes[0] = 0x3A;                                      // neues Telegramm mit 0x3A beginnen
  for (byte n = 0; n < numReceived1; n++) {
    sendBytes[n + 1] = receivedBytes1[n];                   // Die empfangenen Bytes an die nächsten Positionen
  }
  sendBytes[10] = 0x0D;                                     // CR und LF ans Ende
  sendBytes[11] = 0x0A;

  if (tuningState == "AN") {                                // Nur, wenn Tuning via Web-Gui aktiviert ist!!

    if (sendBytes[3] == (0x42) ) {                          // Wenn das 4. Byte 0x42 (2. Stufe) dann
      sendBytes[2] = (0x0B);                                //  3. Byte (Unterstützungslevel)
      sendBytes[3] = (0x46);                                //  4. Byte (Stufe) auf 6
      sendBytes[4] = (0x24);                                // und 5. Byte (Speed) auf 36km/h
    }
  }
}

void Checksumm() {
  sendBytes[8] = 0;
  for (byte n = 1; n < 8; n++) {
    sendBytes[8] = sendBytes[8] + sendBytes[n];
  }
}

void showNewData() {
  if (newData1 == true) {
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
    Serial.println();                                       // bis hier DebugInfo via USB an PC
    Serial1.write(sendBytes, 12);                           // Telegramm rausschicken
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

    Serial.println();                                       // bis hier DebugInfo via USB an PC
  }
}

// Tuning-Loop
void tuning() {
  recvBytesWithStartEndMarkers1();                          // Checks for new message from display
  modNewData();                                             // Does the magic
  Checksumm();                                              // Calculates the checksum
  showNewData();                                            // Sends modified data
  recvBytesWithStartEndMarkers2();                          // Checks for new message from controller
}

void loop() {
  tuning();
}
