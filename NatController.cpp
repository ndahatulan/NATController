#include "NatController.h"

NatController::NatController(uint8_t pinControl, String c_name, uint8_t pwmCh) {
  name = c_name;
  _pwmCh = pwmCh;
  _pinControl = pinControl;
  _pwmEnable = true;
  ledcAttachPin(_pinControl, _pwmCh);
  ledcSetup(_pwmCh, PWM1_Freq, PWM1_Res);  
}

NatController::NatController(uint8_t pinControl, String c_name) {
  name = c_name;
  _pinControl = pinControl;

  pinMode(_pinControl, OUTPUT);

}

void NatController::loop() {
  if (enable) {
    if(mode == MODE_ON) { // ON
        if(!state) {
          turnOn();
          needModePublish = true;
          //
        }
    }
    else if(mode == MODE_OFF) { //OFF
      if(state) {
         turnOff();
         needModePublish = true;
         // Do here after off
      }
    }
    else if(mode == MODE_INT) { // INTERVAL
      if(_intervalCount > 0) { // When switching to interval state. Initial _intervalCount should be 0
        if((_intervalCount % 2) != 0) {   // _intervalCount is odd, odd - on state
          if(getIntervalDuration() >= intervalOn) {
          //if(getIntervalDuration() >= ptr_interval_on) {
            // Do here after 
            turnOff();
            needModePublish = true;
            _intervalMillis = millis(); 
            _intervalCount++;
            Serial.print("Interval Count ");
            Serial.print(_intervalCount);
            Serial.println(" . Interval Pump Off");
          }
        }
        else {  // _intervalCount is even, even-off state
          if(getIntervalDuration() >= intervalOff) {
          //if(getIntervalDuration() >= ptr_interval_off) {
            _intervalMillis = millis(); 
            turnOn();
            needModePublish = true;
            _intervalCount++;
            Serial.print("Interval Count ");
            Serial.print(_intervalCount);
            Serial.println(" . Interval Pump On");
          }
        }
      }
      else {
        if(getIntervalDelayDuration() >= intervalDelayStart) {
          _intervalMillis = millis(); // Timestamp when interval state started
          Serial.println("Interval started");
          //onpump
          turnOn();
          needModePublish = true;
          _intervalCount++;
        }
        
      }
    }
  }
  else {
    if(mode != MODE_OFF) {
      mode = MODE_OFF;
    }
  }

}


void NatController::turnOn() {
  _startMillis = millis();
  if(_pwmEnable) {
    ledcWrite(_pwmCh, pwmDutyCycle);
  }
  else {
    digitalWrite(_pinControl, HIGH);
  }
  
  state = true;
  if(_pwmEnable && pwmDutyCycle == 0) {
    state = false;
  }
}

void NatController::turnOff(){
  if(_pwmEnable) {
    ledcWrite(_pwmCh, 0);
  }
  else {
    digitalWrite(_pinControl, LOW);
  }
  _lastDispensed = getDispensed();
  state = false;
}

unsigned long NatController::getDuration(){
  return millis()-_startMillis;
}

unsigned long NatController::getDispensed(){
  return getDuration()/stepFactor;
}

unsigned long NatController::getLastDispensed(){
  return _lastDispensed;
}

unsigned long int NatController::getIntervalDuration() {
  return (millis() - _intervalMillis)/1000;
}

unsigned long int NatController::getIntervalDelayDuration() {
  return (millis() - _intervalDelayStartMillis)/1000;
}

void NatController::setSpeed(int percentage){
  pwmDutyCycle = map(percentage, 0,100, 0, 255);
}

void NatController::startIntervalMode() {
  _intervalCount = 0;
  mode = MODE_INT;
  _intervalDelayStartMillis = millis();
}
