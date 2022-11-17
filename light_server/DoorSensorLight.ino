/*
 * Door Sensor Light 
 */

#include "RGBLight.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>

#define red 13
#define green 12
#define blue 14

RGBLight light (red, green, blue);

// Variables to hold previous state of the three doors
boolean frontOpen;
boolean backOpen;
boolean extensionOpen;

// WiFi Credentials
const char* ssid = "redacted";
const char* password = "redacted";

// Create server and specify port to listen on (80 - HTTP Default)
ESP8266WebServer server(80);


void setup() {
  
  // Setup Serial Connection
  Serial.begin(115200);
  delay(10);

  // Create Hostname
  WiFi.hostname("doorlight");

  // Connect to WiFi
  Serial.print("\n\nConnecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);

  // Print dot until wifi connected 
  while (WiFi.status() != WL_CONNECTED) {
    light.on("blue");
    Serial.print(".");
    delay(500); 
    light.off();
    delay(500);
  }
  
  Serial.println("");
  Serial.println("WiFi connected");

  light.flashOne("green", 250, 3);
  light.on("green");
  
  // Print the IP address
  Serial.println(WiFi.localIP());

  // Set all doors to closed; 
  frontOpen = false;
  backOpen = false;
  extensionOpen = false;
  
  // Start server
  server.on("/doorstatechange", HTTP_POST, handleDoorStateChange);
  server.begin();
  Serial.println("Server started - Listening for requests....");

  

}


void handleDoorStateChange() {

  // Print recieved POST data
  String pData = server.arg("plain");

  // Return ok status code to server
  server.send(200);
  
  Serial.println("Data Received --> " + pData);
  
  // Create a JSON document to store the parsed JSON
  StaticJsonDocument<200> doorInfo;

  // Parse the POST data to JSON and save pass/fail result in 'error'
  auto error = deserializeJson(doorInfo, pData);

  // If parsing fails then send 400 code (Bad Request) and return 
  if (error) {
    //server.send(400); Removed in case of time delay
    return;
  }
  
  // Ensure that the JSON object has three elements
  if (doorInfo.size() != 3) {
    //server.send(400); Removed in case of time delay
    return;
  }

  // Ensure that the three elements are as expected
  if (!doorInfo.containsKey("front") || !doorInfo.containsKey("back") || !doorInfo.containsKey("extension")) {
    //server.send(400); Removed in case of time delay
    return;
  }

  // Update door info if it has changed
  if (frontOpen != doorInfo["front"] || backOpen != doorInfo["back"] || extensionOpen != doorInfo["extension"]) {
    updateDoorRecord(doorInfo);
  }
}


void updateDoorRecord(StaticJsonDocument<200> doorInfo) {
  frontOpen = doorInfo["front"];
  backOpen = doorInfo["back"];
  extensionOpen = doorInfo["extension"];
}

int openCount() {
  // Counter to count number of doors open
  int openCount = 0;

  if (frontOpen) openCount++;
  if (backOpen) openCount++;
  if (extensionOpen) openCount++;
  
  return openCount;
}


void loop() {
  // The doorlight should constantly handle requests
  server.handleClient();
  delay(10);

  // If all doors are closed then turn the light green
  if (openCount() == 0) {
    light.on("green");
    return;
  }
  
  // Handle Single Door Being Open
  if (openCount() == 1) {
    if (frontOpen) light.on("red");
    if (backOpen) light.on("turquoise");
    if (extensionOpen) light.on("violet");
    return;
  }

  // Handle multiple open doors 
  // If there is more than one door open
  if (openCount() > 1) {
    // For every door 
    for (int i = 0; i < 3; i++) {
      if (frontOpen && i == 0) {
        light.on("red");
        delay(1000);
      }
      
      if (backOpen && i == 1) {
        light.on("turquoise");
        delay(1000);
      }
      
      if (extensionOpen && i == 2) {
        light.on("violet");
        delay(1000);
      } 
    }    
  }
}
