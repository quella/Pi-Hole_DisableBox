/*
This sketch for Arduino IDE configures an ESP8266 to disable the Pi-Hole via API for a specified
period of time.  It uses four buttons (1) 1 minute, (2) 5 Minutes, (3) 10 Minutes, (4) for Enable.
There is an RGB LED for visual BLUE = configuring, RED = Disabled, GREEN = Enabled.

NOTE: I'm still working on setting up the delay and ability to press Enable to cancel a disable request

You need to provide your: SSID, PASSWORD, and IP to PI-HOLE where asked.

*/

#include <Arduino.h>
#include <Wire.h>
#include <ESP8266WiFi.h>

const int button1  = D5;
const int button2  = D6;
const int button3  = D7;
const int button4  = D1;
const int ledred   = D3;
const int ledgreen = D2;
const int ledblue = D4;

unsigned long timeout;

int buttonpressed;
int seconds;

uint16_t waitseconds = 0;      // max == 65535
uint32_t lastTime;

const char* ssid     = "YOUR_WIRELESS_SSID";
const char* password = "YOUR_WIRELESS_PASSWORD";

const char* host = "IP_ADDRESS_TO_PI-HOLE";

void setup() {

  pinMode(button1, INPUT);
  digitalWrite(button1, LOW);
  pinMode(button2, INPUT);
  digitalWrite(button2, LOW);
  pinMode(button3, INPUT);
  digitalWrite(button3, LOW);
  pinMode(button4, INPUT);
  digitalWrite(button4, LOW);
  pinMode(ledred, OUTPUT);
  pinMode(ledgreen, OUTPUT);
  pinMode(ledblue, OUTPUT);

  digitalWrite(ledblue, HIGH);
      
  Serial.begin(115200);
  delay(10);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

delay(5000);

  digitalWrite(ledblue, LOW);
  digitalWrite(ledgreen, HIGH);

}

void loop() {
 
buttonpressed = 0;
seconds = 0;
   
  //
  //  Button Wait Loop
  //
  
  if (digitalRead(button1) == HIGH)
    {
    buttonpressed = 1;
    seconds = 60;       // 60 Seconds
    waitseconds = 60000;
    callapi();
    disabled();
    }
 
  if (digitalRead(button2) == HIGH) 
    {
    buttonpressed = 2;
    seconds = 300;      // 5 minutes
    waitseconds = 300000;
    callapi();
    disabled();
    }

  if (digitalRead(button3) == HIGH) 
    {
    buttonpressed = 3;
    seconds = 600;      // 10 minutes
    waitseconds = 600000;
    callapi();
    disabled();
    }

  if (digitalRead(button4) == HIGH) 
    {
    buttonpressed = 4;
    seconds = 0;        //Not needed for ENABLE
    waitseconds = 1;
    callapi();
    disabled();
    }

   delay(200);  //Delay Button Reads
}

//
// API Call to Pi-Hole to either disable ot enable
//

void callapi() {

String url;

    digitalWrite(ledred, LOW);
    digitalWrite(ledgreen, LOW);
    digitalWrite(ledblue, HIGH);

//
// Build a URI for the request to the EmonCMS server
//
  
  if (buttonpressed == 4) {
     url = "/admin/api.php?enable&auth=API_KEY";
  }
  else 
     url = "/admin/api.php?disable=" + String(seconds) + "&auth=API_KEY";

//
// Use WiFiClient class to create TCP connections
//
  Serial.print("connecting to ");
  Serial.println(host);

  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }

//
// This will send the request to the server
//
                 
  client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + host + " \r\n" + "Connection: close\r\n\r\n");
  
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }

//
// Read all the lines of the reply from server and print them to Serial
//

  while (client.available()) {
      String line = client.readStringUntil('\r');
      Serial.print(line);
    }
  
    Serial.println();
    Serial.println("closing connection");
  }

  void disabled() {
    Serial.println(" ");
    Serial.println("Entered disabled loop");
    Serial.print("Button Pressed: ");
    Serial.println(buttonpressed);

    digitalWrite(ledblue, LOW);
    digitalWrite(ledgreen, LOW);
    digitalWrite(ledred, HIGH);


    delay(waitseconds);
    
    digitalWrite(ledgreen, HIGH);
    digitalWrite(ledred, LOW);
   
   }
