// Import libraries
#include <WiFi.h>

// WiFi-AP SSID & Password
const char* ssid = "Moscow Plus";
const char* password = "Moscow1234";

// IP-Configuration
IPAddress local_ip(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
WiFiServer server(80);

// Stores the HTTP request
String header;

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

void setup() {
  // Serial configuration
  Serial.begin(115200);                                    // USB-Serial console
  Serial1.begin(9600, SERIAL_8N1, 12, 13);                 // Additional consoles for display and controller communication
  Serial2.begin(9600, SERIAL_8N1, 14, 15);

  // Starting WiFi-AP
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  server.begin();

  // Show on console when booted
  Serial.println("-= NCM Moscow Plus Controller ready =-");
  Serial.println("  -= WiFi-AP & HTTP server started =-");
}

// Tuning-Loop
void loop() {
  recvBytesWithStartEndMarkers1();                          // Checks for new message from display
  modNewData();                                             // Does the magic
  Checksumm();                                              // Calculates the checksum
  showNewData();                                            // Sends modified data
  recvBytesWithStartEndMarkers2();                          // Checks for new message from controller

  // WiFi-Loop
  WiFiClient client = server.available();                   // Checks for new Webclient
  if (client) {                                             // If client ist connected
    String currentLine = "";                                // String with client Data
    while (client.connected()) {                            // Loop while client ist connected
      if (client.available()) {                             // If recieving data from client
        char c = client.read();                             // read the bytes
        header += c;
        if (c == '\n') {                                    // If the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // Turns tuning ON and OFF
            if (header.indexOf("GET /tuning/an") >= 0) {
              Serial.println("Tuning on");
              tuningState = "AN";
            } else if (header.indexOf("GET /tuning/aus") >= 0) {
              Serial.println("Tuning off");
              tuningState = "AUS";
            }

            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #FF0000; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #4CAF50;}</style></head>");

            // Web Page Heading
            client.println("<body><h1>Moscow Plus Controller</h1>");

            // Display current state, and ON/OFF buttons
            client.println("<p>Tuning ist:</p>");
            if (tuningState == "AN") {
              client.println("<p><a href=\"/tuning/aus\"><button class=\"button\">AN</button></a></p>");
            } else {
              client.println("<p><a href=\"/tuning/an\"><button class=\"button button2\">AUS</button></a></p>");
            }
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
  }
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
        receivedBytes2[ndx - 1] = '\0'; // terminate the string
        recvInProgress = false;
        numReceived2 = ndx - 1; // save the number for use when printing
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
  sendBytes[0] = 0x3A;                        // neues Telegramm mit 0x3A beginnen
  for (byte n = 0; n < numReceived1; n++) {
    sendBytes[n + 1] = receivedBytes1[n];      // Die empfangenen Bytes an die nächsten Positionen
  }
  sendBytes[10] = 0x0D;                        // CR und LF ans Ende
  sendBytes[11] = 0x0A;

  if (tuningState == "AN") {                      // Nur, wenn Tuning via Web-Gui aktiviert ist!!

    if (sendBytes[3] == (0x42) ) {                // Wenn das 4. Byte 0x42 (2. Stufe) dann
      sendBytes[2] = (0x0B);                      //  3. Byte (Unterstützungslevel)
      sendBytes[3] = (0x46);                      //  4. Byte (Stufe) auf 6
      sendBytes[4] = (0x24);                      // und 5. Byte (Speed) auf 36km/h
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
    Serial.println();                          // bis hier DebugInfo via USB an PC



    Serial1.write(sendBytes, 12);             // Telegramm rausschicken
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

    Serial.println();                          // bis hier DebugInfo via USB an PC
  }
}
