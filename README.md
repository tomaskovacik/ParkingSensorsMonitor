# ParkingSensorsMonitor

This is library to read data send by cheap parking sensors on monitor line, probably most units with some sort of "monitor" send same data. 

This is example of board with desired functionality:

<img src="https://raw.githubusercontent.com/tomaskovacik/ParkingSensorsMonitor/master/pics/20191115_175723.jpg" />

ant here is one which was in same plastic box but luck funkcionality:

<img src="https://raw.githubusercontent.com/tomaskovacik/ParkingSensorsMonitor/master/pics/20191115_175726.jpg" />

Protocol used in comunication between parking sensor unit and monitor is pulse width modulation at 5V level. Packet is 32bits long from each 8bits represent real value of distance measured by sensor, expect 0xFF which represent infinit distance => or no obsticle detected by sensor.

<img src="https://raw.githubusercontent.com/tomaskovacik/ParkingSensorsMonitor/master/pics/pulseview1.png" />

Packet start with almost 2ms long pulse (this lib use pulse longer then 1500us)

<img src="https://raw.githubusercontent.com/tomaskovacik/ParkingSensorsMonitor/master/pics/start_pulse.png" />

Logic zero is represented with 100us pulse:

<img src="https://raw.githubusercontent.com/tomaskovacik/ParkingSensorsMonitor/master/pics/100us.png" />

Logic one is represented with 200us pulse:

<img src="https://raw.githubusercontent.com/tomaskovacik/ParkingSensorsMonitor/master/pics/200us.png" />

This library require pin with  external interupt (INTx) to be used, also TIMER2 is used (ticks at 0.5us + compare ISR is fired evry 10 ticks, so precition is 5us) is used for timing pulses,any timer can be used if changed in code and configured correctly..

When library receive all 32bits of data calling available() function will return true and data can be read by 
 
 - read() function, this function also clear internal attribute indication that new data are available
 or
 - calling getDistance(sensor_number_from_1_to_4) or getDistanceInMeters(sensor_number_from_1_to_4) function will return only data for specified sensor.
 
 Calling clearNewData() function will clearn internal attribute about received new data. Read() function clearn thisn attribute automaticaly.
 
