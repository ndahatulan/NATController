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
#define MODE_DUR 2
#define MODE_ML 3
#define MODE_AUTO 4
#define MODE_INT 5
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
  //unsigned int ptr_interval_on;
  //unsigned int ptr_interval_off;
  unsigned int intervalOn; 
  unsigned int intervalOff;
  bool needModePublish = false;
  unsigned int intervalDelayStart = 10;

  void loop();
  void turnOn();
  void turnOff();
  unsigned long int getDuration(); // Get current duration. millis - the startMillis.
  unsigned long int getDispensed();
  unsigned long int getLastDispensed();
  void setSpeed(int percentage);
  unsigned long int getIntervalDuration();
  unsigned long int getIntervalDelayDuration();
  void startIntervalMode();
  
	
  private:
  bool _pwmEnable = false;
  uint8_t _pwmCh = 0;
  unsigned long int _pinControl; // Pin number to switch Pump
  unsigned long int _lastDispensed;
  unsigned long int _startMillis = 0; // duration in millis since the pump started
  unsigned int _intervalCount = 0;
  unsigned long int _intervalMillis;
  unsigned long int _intervalDelayStartMillis;
		
};

#endif
