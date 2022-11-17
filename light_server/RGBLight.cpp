#include "RGBLight.h"

// Constructor for RGB Light. Takes in pin numbers for the light
RGBLight :: RGBLight (int rPin, int gPin, int bPin) {
  this -> rPin = rPin;
  this -> gPin = gPin;
  this -> bPin = bPin;
  init ();
}

// initialises the three light GPIO pins
void RGBLight :: init () {
  pinMode (rPin, OUTPUT);
  pinMode (gPin, OUTPUT);
  pinMode (bPin, OUTPUT);
  off();
}

// Turns the light off 
void RGBLight :: off () {
  digitalWrite (rPin, LOW);
  digitalWrite (gPin, LOW);
  digitalWrite (bPin, LOW);
}

// Turns the light on - sets to specified colour
void RGBLight :: on (String colour) {
  // Red
  if (colour.indexOf("red") >= 0) {
    off ();
    digitalWrite (rPin, HIGH);
  } 

  // Green
  else if (colour.indexOf("green") >= 0) {
    off ();
    digitalWrite (gPin, HIGH);
  } 

  // Blue
  else if (colour.indexOf("blue") >= 0) {
    off ();
    digitalWrite (bPin, HIGH);
  } 

  // Yellow
  else if (colour.indexOf("yellow") >= 0) {
    off ();
    digitalWrite (rPin, HIGH);
    digitalWrite (gPin, HIGH);
  } 

  // Turquoise
  else if (colour.indexOf("turquoise") >= 0) {
    off ();
    digitalWrite (gPin, HIGH);
    digitalWrite (bPin, HIGH);
  } 

  // Violet 
  else if (colour.indexOf("violet") >= 0) {
    off ();
    digitalWrite (rPin, HIGH);
    digitalWrite (bPin, HIGH);
  } 

  // White
  else if (colour.indexOf("white") >= 0) {
    off ();
    digitalWrite (rPin, HIGH);
    digitalWrite (gPin, HIGH);
    digitalWrite (bPin, HIGH);
  }
}

// Flashes a colour a set number of times with a set interval between flashes
void RGBLight :: flashOne (String colour, int waitTime, int repetitions) {
  for (int i = 0; i < repetitions; i++) {
    on (colour);
    delay (waitTime);
    off();
    delay (waitTime);
  }
}

// Flashes two colours a set number of times with a set delay
void RGBLight :: flashTwo (String colourOne, String colourTwo, int waitTime, int repetitions) {
  for (int i = 0; i < repetitions; i++) {
    on (colourOne);
    delay(waitTime);
    off();
    on (colourTwo);
    delay(waitTime);
    off();
  }
}
