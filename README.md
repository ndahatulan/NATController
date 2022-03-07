



# Description

NAT Controller for pumps.


## Features
* ON
* OFF
* DURATION - TURN ON on a specified time in seconds
**   durationValue - Time in seconds to dispense
* STEP - Dispense by STEP
**   stepValue - Turn ON by # of STEPS
* INTERVAL - TURN ON or OFF on a specified time interval.
**   startIntervalMode() - To trigger Interval Mode.
**   intervalOn - Turn ON in seconds.
**   intervalOff - Turn OFF in seconds.
**   intervalDelayStart - Delay before triggering interval mode in seconds. default is 10 seconds.
* AUTO - TURN ON if min value is reached. TURN OFF if max value is reached. Ideal for automation with sensors.
**   startAutoMode() - To trigger AUTO Mode.
**   float autoValue - Sensor value
**   float autoMin - If autoValue is <= autoMin, turn ON. If (autoValue = autoMin = autoMax), it will not turn ON.
**   float autoMax - If autoValue is >= autoMax, turn OFF
**   int autoDelayStart - Delay turn ON in seconds. default is 10 seconds.

