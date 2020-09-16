# ParkingSensorsMonitor

This repository contain library to read data send by cheap parking sensors on monitor line, probably most units with some sort of "monitor" send the same data. 

This is example of boards with desired functionality:

<img src="https://raw.githubusercontent.com/tomaskovacik/ParkingSensorsMonitor/master/pics/20191115_175723.jpg" />
<a href="https://www.aliexpress.com/item/32729131286.html">@ali</a>

anothere one which works (DA is data out):
<img src="https://raw.githubusercontent.com/tomaskovacik/ParkingSensorsMonitor/master/pics/20200525_161259.jpg" />
(I cannot find listing for this kit ..  but this one looks like it: <a href="https://www.aliexpress.com/item/32573184583.html">@ali</A>)

and here is one which was in same plastic box but luck funkcionality, most notable diffrence between previous one is that this one luck separate buzzer output connector:
<img src="https://raw.githubusercontent.com/tomaskovacik/ParkingSensorsMonitor/master/pics/20191115_175726.jpg" />

The protocol used in communication between the parking sensor unit and the monitor is pulse width modulation at 5V level.
The packet is (sort of) 32bits long, each 8bits represent the real value of distance measured by sensor, expect 0xFF which represent infinite distance => or no obstacle detected by the sensor.
In my unit sensors are send as: S1, S4, S3 and S2.

<img src="https://raw.githubusercontent.com/tomaskovacik/ParkingSensorsMonitor/master/pics/pulseview1.png" />

Packet start with almost 2ms long pulse (this lib use pulse longer then 1500us) and then one 100us pulse, this is why library count bits to 33 not 32. 

<img src="https://raw.githubusercontent.com/tomaskovacik/ParkingSensorsMonitor/master/pics/start_pulse.png" />

Logic zero is represented with 100us pulse:

<img src="https://raw.githubusercontent.com/tomaskovacik/ParkingSensorsMonitor/master/pics/100us.png" />

Logic one is represented with 200us pulse:

<img src="https://raw.githubusercontent.com/tomaskovacik/ParkingSensorsMonitor/master/pics/200us.png" />

This library requires pin with external interrupt (INTx), also TIMER2 is used for timing of pulses, timer ticks at 0.5us and compare ISR is fired every ten ticks, so precision is 5us. But any timer can be used if changed in code and configured correctly.

When the library receives all 32bits of data then calling available() function will return true, and data can then be read by calling functions:
 
 - getDistance(sensor)
 - getDistanceInMeters(sensor),this function will return only data for specified sensor.
 
Calling clearNewData() after reading data clear internal attribute which is set when all data are received.

#How to connect

Connect ground together.
Connect arduino pin with external **INT**errupt to "data" pin.

[powerGround NC power+12V][ground data +5V buzzer][emptyslot][S1][S2][S3][S4]
