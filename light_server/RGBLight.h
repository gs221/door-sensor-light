#include <Arduino.h>

class RGBLight {
  private:
    int rPin;
    int gPin;
    int bPin;
    
  public:
    RGBLight (int rPin, int gPin, int bPin);
    void init ();
    void off ();
    void on (String colour);
    void flashOne (String colour, int waitTime, int repetitions);
    void flashTwo (String colourOne, String colourTwo, int waitTime, int repetitions);
};
