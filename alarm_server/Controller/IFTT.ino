  const String NOTIFY_DOOR_OPEN_IFTT_ADDRESS = "redacted";
  const String NOTIFY_ALARM_STATE_IFTT_ADDRESS = "redacted";

  // Send push notification to indicate that a door has been opened
  void notify_DoorOpen(String door) {
      // If the alarm is armed 
      if (alarmState == ARMED) {
           // Create URL for GET request 
           String Data = "?value1=" + door;
  
           // Send request to IFTT 
           HTTPClient http;
           String request = NOTIFY_DOOR_OPEN_IFTT_ADDRESS + Data;
           http.begin(request);
           http.GET();
           http.end();
      }
  }
  
  // Send push notification with the alarm state (Armed/Disarmed)
  void notify_AlarmState() {

    // Convert boolean alarm state to string message
    String stateMsg = "UNKNOWN ERROR: Alarm is neither armed or disarmed";
    
    if (alarmState == ARMED) {
      stateMsg = "Armed";
    } else if (alarmState == DISARMED) {
      stateMsg = "Disarmed";
    }
    
    // Create URL for GET request 
    String Data = "?value1=" + stateMsg;
  
    // Send request to IFTT 
    HTTPClient http;
    String request = NOTIFY_ALARM_STATE_IFTT_ADDRESS + Data;
    http.begin(request);
    http.GET();
    http.end();
  }
