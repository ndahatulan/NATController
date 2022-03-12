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
    /* MODE ON - 1*/
    if(mode == MODE_ON) {
        if(!state) {
          turnOn();
          needModePublish = true;
          //
        }
    }
    /* MODE OFF - 0*/
    else if(mode == MODE_OFF) {
      if(state) {
         turnOff();
         needModePublish = true;
         // Do here after off
      }
    }

    /* MODE  - 2*/
    else if(mode == MODE_DUR) {
      if(state) {
        if(getDuration() > durationValue * 1000) {
          turnOff();
          needStatePublish = true;
          mode = MODE_OFF;
          //pump1.offPump();
          //publishPumpState();
          //settings.PUMP1_MODE = PUMP1_MODE_OFF;
          //pump1ProperOff();
        }
      }
      else {
        if(durationValue > 0) {
          //_durationMillis = millis();
          turnOn();
          needStatePublish = true;
          //pump1.onPump();
          //publishPumpState();
        }
        else { // if durationValue = 0 Change to mode to OFF
          mode = MODE_OFF;
          needModePublish = true;
        }
      }
    }

    /* MODE  - 3*/
    else if(mode == MODE_STEP) {
      if(state) {
        if(getDispensed() > stepValue) {
          turnOff();
          needStatePublish = true;
          mode = MODE_OFF;
          //pump1.offPump();
          //publishPumpState();
          //settings.PUMP1_MODE = PUMP1_MODE_OFF;
          //pump1ProperOff();
        }
      }
      else {
        if(stepValue > 0) {
          //_durationMillis = millis();
          turnOn();
          needStatePublish = true;
          //pump1.onPump();
          //publishPumpState();
        }
        else { // if stepValue = 0 Change to mode to OFF
          mode = MODE_OFF;
          needModePublish = true;
        }
      }
    }
    
    /* MODE AUTO - 4*/
    else if(mode == MODE_AUTO || mode == MODE_AWTP) {
      if(autoValue <= autoMin && state == false) { // if autoValue is out of range and state is off
        if (!(autoMin == autoMax && autoValue == autoMin)) { // automin, automax and autovalue should not be equal
          if(_autoDelay == false) { // Start delay
            _autoDelayStartMillis = millis(); // Start timer for delay
            _autoDelay = true; // Set PumpDelay state to true
            Serial.println("_autoDelay true");
          }
      
          if(_autoDelay == true && millis()-_autoDelayStartMillis >= autoDelayStart*1000) { // If delay state is true and already reached the delay timer
            turnOn();
            Serial.println("S1 turned ON");
            // publishPumpState();
            _autoDelay = false;
          }
        }
      }
      else if(autoValue > autoMin && state == false) {
        _autoDelay = false;
      }
      else if(autoValue >= autoMax && state == true) {
        turnOff();
        Serial.println("S1 turned OFF");
        Serial.print("_autoDelay: ");
        Serial.println(_autoDelay);
        //pump1.offPump();
        //publishPumpState();
        //pump1ProperOff();
        //Serial.println("");
        //Serial.print("Dispensed: "); 
        //Serial.println(pump1.getDispensed());
        // Send pump dispense if required
        
      }
  
      if (state == true) { // If already reached maximum pump time, turn off PUMP
        if (getDuration() >= autoTimeMax*1000) {
          mode == MODE_OFF;
         // settings.PUMP1_MODE = PUMP1_MODE_OFF;
         // pump1ProperOff();
          //globalconfig.settingsDoc["PUMP1_MODE"] = settings.PUMP1_MODE;
          //globalconfig.save();
          //Serial.println("");
          //Serial.print("Dispensed: "); 
          //Serial.println(pump1.getDispensed());
          // Send pump dispensed if required
          
        }
      }
    }
    /* MODE INTERVAL - 5*/
    else if(mode == MODE_INT) {
      if(_intervalCount > 0) { // When switching to interval state. Initial _intervalCount should be 0
        if((_intervalCount % 2) != 0) {   // _intervalCount is odd, odd - on state
          if(getIntervalDuration() >= intervalOn) {
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
    /* MODE VALUE not on the option*/
    else {
      mode == MODE_OFF;
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

void NatController::startDurationMode() {
  mode = MODE_DUR;
  if(state) {
    turnOff();
  }
}

void NatController::startStepMode() {
  mode = MODE_STEP;
  if(state) {
    turnOff();
  }
}

void NatController::startIntervalMode() {
  _intervalCount = 0;
  if(state) {
    turnOff();
  }
  mode = MODE_INT;
  _intervalDelayStartMillis = millis();
}

void NatController::startAutoMode() {
  mode = MODE_AUTO;
  if(state) {
    turnOff();
  }
}

void NatController::startAutoAWTPMode() {
  mode = MODE_AWTP;
  if(state) {
    turnOff();
  }
}
