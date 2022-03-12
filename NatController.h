/*
  Component Library Template
 Controller for pump, perlistatic
*/
#ifndef NatController_h
#define NatController_h

#include "Arduino.h"

#define PWM1_Ch    0
#define PWM1_Res   8
#define PWM1_Freq  1000

#define PWM1 0
#define PWM2 1
#define PWM3 2
#define PWM4 3

#define PWM_0 0
#define PWM_25 64
#define PWM_75 127
#define PWM_100 255

#define MODE_OFF 0
#define MODE_ON 1
#define MODE_DUR 2 // TURN ON by duration in seconds
#define MODE_STEP 3 // Turn ON by # of STEPS
#define MODE_AUTO 4 // AUTO Water Level
#define MODE_AWLV 4 // AUTO Water Level
#define MODE_INT 5  // Interval Mode
#define MODE_ATMP 6 // AUTO Temperature. Not Yet Implemented
#define MODE_AHMD 7 // AUTO Humidity. Not Yet Implemented
#define MODE_AWTP 8 // AUTO Water Temp

/*
PWM
0 - 0 percent
64 - 25 percent
127 - 75 percent
255 - 100 percent
*/
//
class NatController
{
  public:
  NatController(uint8_t pinControl, String c_name, uint8_t pwmCh);
  NatController(uint8_t pinControl, String c_name);
  unsigned long int stepFactor=1000; // step number, like 1 one ml per step
  bool enable = true;
  bool state = 0; // if on or off
  String name; // name for label
  int pwmDutyCycle = 255;
  int mode = MODE_OFF;
  bool needModePublish = false;
  bool needStatePublish = false;
  //unsigned int ptr_interval_on;
  //unsigned int ptr_interval_off;
  unsigned int durationValue = 30;
  unsigned int stepValue = 1000;
  unsigned int intervalOn = 60; // 1 minute
  unsigned int intervalOff = 180; // 3 minutes
  unsigned int intervalDelayStart = 10; // Delay in seconds before starting the interval on
  float autoValue = 2.00;
  float autoMin = 1.00;
  float autoMax = 3.00;
  unsigned int autoDelayStart = 10; // Delay in seconds before starting the auto mode
  unsigned long int autoTimeMax = 3600; // Maximum time in seconds that state is ON when auto mode.

  void loop();
  void turnOn();
  void turnOff();
  unsigned long int getDuration(); // Get current duration. millis - the startMillis.
  unsigned long int getDispensed(); // 
  unsigned long int getLastDispensed();
  void setSpeed(int percentage);
  unsigned long int getIntervalDuration();
  unsigned long int getIntervalDelayDuration();
  void startStepMode();
  void startDurationMode();
  void startIntervalMode();
  void startAutoMode();
  void startAutoAWTPMode(); 
	
  private:
  bool _pwmEnable = false;
  uint8_t _pwmCh = 0;
  unsigned long int _pinControl; // Pin number to switch Pump
  unsigned long int _lastDispensed;
  
  //unsigned long int _durationMillis;
  unsigned long int _startMillis = 0; // start duration in millis when turn ON
  unsigned int _intervalCount = 0;
  unsigned long int _intervalMillis;
  unsigned long int _intervalDelayStartMillis;
  //unsigned int _autoCount;
  bool _autoDelay; // true id on auto delay state
  //unsigned long int _autoMillis;
  unsigned long int _autoDelayStartMillis;
  bool _autoOnIncrease = true; // true: Turn ON to increase autoValue. false: autoMin > autoMax, Turn ON to decrease autoValue.
		
};

#endif
