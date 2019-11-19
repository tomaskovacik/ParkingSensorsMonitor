# ParkingSensorsMonitor

This repository contain library to read data send by cheap parking sensors on monitor line, probably most units with some "monitor" send the same data. 

This is example of board with desired functionality:

<img src="https://raw.githubusercontent.com/tomaskovacik/ParkingSensorsMonitor/master/pics/20191115_175723.jpg" />

ant here is one which was in same plastic box but luck funkcionality:

<img src="https://raw.githubusercontent.com/tomaskovacik/ParkingSensorsMonitor/master/pics/20191115_175726.jpg" />

The protocol used in communication between the parking sensor unit and the monitor is pulse width modulation at 5V level. The packet is 32bits long, each 8bits represent the real value of distance measured by sensor, expect 0xFF which represent infinite distance => or no obstacle detected by the sensor.

<img src="https://raw.githubusercontent.com/tomaskovacik/ParkingSensorsMonitor/master/pics/pulseview1.png" />

Packet start with almost 2ms long pulse (this lib use pulse longer then 1500us)

<img src="https://raw.githubusercontent.com/tomaskovacik/ParkingSensorsMonitor/master/pics/start_pulse.png" />

Logic zero is represented with 100us pulse:

<img src="https://raw.githubusercontent.com/tomaskovacik/ParkingSensorsMonitor/master/pics/100us.png" />

Logic one is represented with 200us pulse:

<img src="https://raw.githubusercontent.com/tomaskovacik/ParkingSensorsMonitor/master/pics/200us.png" />

This library requires pin with external interrupt (INTx), also TIMER2 is used for timing of pulses, timer ticks at 0.5us + compare ISR is fired every ten ticks, so precision is 5us. But any timer can be used if changed in code and configured correctly.

When the library receives all 32bits of data calling available() function will return true, and data can then be read by calling on of this functions:
 
 - read(), this function also clear internal attribute indication that new data are available
 or
 - getDistance(sensor) or getDistanceInMeters(sensor),this function will return only data for specified sensor.
 
Using read() function does not require calling clearNewData() function,  which clear internal attribute about received new data. On the other side, using getDistance(sensor) or getDistanceInMeters(sensor) require to call  clearNewData() after reading data.
