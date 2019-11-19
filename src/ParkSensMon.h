/*
        GNU GPL v3
        (C) Tomas Kovacik [nail at nodomain dot sk]
        https://github.com/tomaskovacik/

        library to read chines parking sensor module with monitor 
        requirments:
                -  pin with INT
                - one timer (TIMER2)

        data are send as PWM pulses
        ~ 1500us is start pulse
        < 150us logic 0
        > 150us logic 1 

        whole packet has 32bits, each 8bits represent distance measured by sensor,in 10cm  steps
        0xFF represent more then 2.5m (no obsticle)

        on my module pinout was like:

        +-------------------------------------------------------------------------------+
        |                                                                               |
        | [powerGround NC power+12V][ground data +5V buzzer][emptyslot][S1][S2][S3][S4] |
        +-------------------------------------------------------------------------------+

        howto connect:
        grounds together
        arduino pin with external INTerrupt to "data"

*/


#ifndef ParkSensMon_H
#define ParkSensMon_H
#include <Arduino.h>

volatile static uint8_t __bits = 0;
volatile static uint8_t __newData = 0;
//volatile static uint8_t __enableCounting=0;
volatile static uint16_t __pulses = 0;
volatile static uint32_t __data;

class ParkSensMon
{
  public:
	ParkSensMon(uint8_t inpin);
	~ParkSensMon();
	void begin();
	uint32_t read(void);
	uint8_t available();
	uint8_t getDistance(uint8_t sensor);
	float getDistanceInMeters(uint8_t sensor);
	void clearNewData(void);
	static void startCountWidthOfPulse(void);
	static void countWidthOfPulse(void);
  private:
	uint8_t __inPin;
};
#endif
