#include <WiFi101.h>
#include "arduino_secrets.h"

const int tempSensorPin = A0;

char ssid[] = SECRET_SSID;
char key[] = SECRET_KEY;

char server[] = "172.16.83.121"; // change this to your server IP

const int sendInterval = 120000;

// Initialize the Ethernet client library
WiFiClient client;

void setup() {
  pinMode(tempSensorPin, INPUT);
  Serial.begin(9600);
  connectToWiFiNetwork();
}

void loop() {
  float currentTemperature = readTemperature(); // measure temperature
  postTemperatureToServer(currentTemperature); // and post it to server
  delay(sendInterval);
}

void connectToWiFiNetwork() {
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  int status = WL_IDLE_STATUS; 
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WEP network, SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, key);
    delay(5000);
  }
  Serial.println("You are connected");
  printWiFiData();
}

void printWiFiData() {
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
}

void postTemperatureToServer(float currentTemperature) {
  String postData = "temperature=";
  postData.concat(currentTemperature);
  postData.concat("&room=kitchen");

  String host = String("Host: ") + String(server);
  
  if (client.connect(server, 3000)) {
    Serial.println("Making request.");
    // Make an HTTP request:
    client.println("POST /sensor/temp HTTP/1.1");
    client.println(host);
    client.println("User-Agent: Arduino/1.0");
    client.println("Connection: close");
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.print("Content-Length: ");
    client.println(postData.length());
    client.println();
    client.println(postData);
    client.print("&");
    client.print("room=kitchen"); // also send room where measurement occurred (for future extensions)
    client.println();
  }
  delay(500);
  while (client.available()) {
    char c = client.read();
    Serial.write(c);
  }
  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    Serial.println("Request complete. Disconnecting from server.");
    client.stop();
  }
}

float readTemperature() {
  float voltage = (analogRead(tempSensorPin) / 1024.0) * 3.3;
  float temperatureC = (voltage - 0.5) * 100 ;
  Serial.print("Current temp is: ");
  Serial.println(temperatureC);
  return temperatureC;
}
