#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

// Converts GPIO pin numbers to the labels on the 'wemos' board 
// D(x) refers to door (number), not the pins on the wemos board 
#define D1 5
#define D2 14
#define D3 4
#define B1 2 

// Sets the delay between loops 
#define Delay 100

// Defines open and closed signal values
#define OPEN 1
#define CLOSED 0

// Defines button press signal values 
#define BUTTON_NOT_PRESSED 1
#define BUTTON_PRESSED 0

// Defines alarm state values 
#define DISARMED 0
#define ARMED 1

// Flag determines whether or not push notification is to be sent
volatile boolean disarmButtonPressed;

// Global States - Used to determine Door and Alarm status 
int alarmState;
int d1State;
int d2State;
int d3State;
int b1State;

//WiFi Credentials
const char* ssid = "redacted";
const char* password = "redacted";

//Create server and specify port to listen on (80 - HTTP Default)
WiFiServer server(80);


// ------------------ START OF ALARM APPLICATION ----------------- \\


/*
 * Setup Function:
 *   - Initialises WIFI
 *   - Sets up GPIO pins correctly with pullup resistors
 *   - Initialises serial connection 
 *   - Initialises door and button states
 */
void setup() {

  // Setup Serial Connection
  Serial.begin(115200);
  Serial.println();
  Serial.println("Serial started at 115200");
  Serial.println();
  
  //Create Hostname
  WiFi.hostname("Alarm");

  //Connect to WiFi
  Serial.print("\n\nConnecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);

  //Print dot until wifi connected 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected");
  
  // Start server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());
  
  // GPIO Setup
  pinMode(D1, INPUT_PULLUP);
  pinMode(D2, INPUT_PULLUP);
  pinMode(D3, INPUT_PULLUP);
  pinMode(B1, INPUT_PULLUP);

  // Initialise States
  disarmButtonPressed = false;
  alarmState = DISARMED;
  d1State = CLOSED;
  d2State = CLOSED;
  d3State = CLOSED;
  b1State = BUTTON_NOT_PRESSED;

  // Setup Interrupts 
  attachInterrupt(digitalPinToInterrupt(B1), handleButtonPress, FALLING);
}

/*
 * Main Program Loop:
 */
void loop() {
  // Delay Loop
  delay(Delay);

  // Check if any door state has changed 
  doorCheck();

  // If the arm/disarm button is pressed, change the armed state and sent push notification
  if (disarmButtonPressed) {
    delay(1000);
    alarmState = !alarmState;
    notify_AlarmState();
    disarmButtonPressed = false;
  }  
}


/*
 * Front Door Open:
 * Triggered when D1 pin value RISES 0 -> 1 
 */
void doorCheck() {
    // Check Front Door (D1)
    // If there is a change in state 
    if (d1State != digitalRead(D1)) {
      // Handle Door Open
      if (digitalRead(D1) == OPEN) {
        Serial.println("INFO: Front Door Open");
        d1State = OPEN;
        postDoorInfo();
        notify_DoorOpen("Front%20Door");
      }
      // Handle Door Closed
      if (digitalRead(D1) == CLOSED) {
        Serial.println("INFO: Front Door Closed");
        d1State = CLOSED;
        postDoorInfo();
      }
    }
    
    // Check Extension Door (D2)
    // If there is a change in state 
    if (d2State != digitalRead(D2)) {
      // Handle Door Open
      if (digitalRead(D2) == OPEN) {
        Serial.println("INFO: Extension Door Open");
        d2State = OPEN;
        postDoorInfo();
        notify_DoorOpen("Extension%20Door");
      }
      // Handle Door Closed
      if (digitalRead(D2) == CLOSED) {
        Serial.println("INFO: Extension Door Closed");
        d2State = CLOSED;
        postDoorInfo();
      }
    }
    
    // Check Back Door (D3)
    // If there is a change in state 
    if (d3State != digitalRead(D3)) {
      // Handle Door Open
      if (digitalRead(D3) == OPEN) {
        Serial.println("INFO: Back Door Open");
        d3State = OPEN;
        postDoorInfo();
        notify_DoorOpen("Back%20Door");
      }
      // Handle Door Closed
      if (digitalRead(D3) == CLOSED) {
        Serial.println("INFO: Back Door Closed");
        d3State = CLOSED;
        postDoorInfo();
      }
    }
}


/*
 * If the arm/disarm button is pressed, the boolean flag is updated
 * accordingly.
 */
 ICACHE_RAM_ATTR void handleButtonPress() {
  disarmButtonPressed = true;
}


/*
 * Returns correctly formatted JSON String with the current status of each door
 */
String getJSON() {
    String json = "{ \"front\" : ";
    json += d1State;
    json += ", \"back\" :";
    json += d2State;
    json += ", \"extension\" : ";
    json += d3State;
    json += " }";
  
    return json;
}


/*
 * POSTs the current door states to the doorlight 
 */
 void postDoorInfo() {
    HTTPClient http;
      
    String address = "http://doorlight/doorstatechange";
    
    http.begin(address);
    http.addHeader("Content-Type", "application/json");
    
    int httpResponseCode = http.POST(getJSON());
  
    http.end();
  
    if (httpResponseCode == 200) {
      Serial.println("Data sent to server --> " + getJSON());
    } else {
      Serial.println("ERROR: Data was not sent to server");
    }
 }
