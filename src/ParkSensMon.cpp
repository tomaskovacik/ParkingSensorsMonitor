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


#include "ParkSensMon.h"
#include <Arduino.h>

ParkSensMon::ParkSensMon(uint8_t inpin)
{
	__inPin = inpin;	
}

ParkSensMon::~ParkSensMon(){}


void ParkSensMon::begin()
{
  //timer2 @ 0.5us ticks
  TCCR2A = 0x00; // Normal port operation, OC0 disconnected
  TCCR2B = 0x00; // Normal port operation, OC0 disconnected
  TCCR2A |= _BV(WGM21); // CTC mode
  TCCR2B |= _BV(CS21);// prescaler = 8 -> 1 timer clock tick is 0.5us long @ 16Mhz
  OCR2A = 10;//run compare rutine every 5us, 0.5x10
  TCNT2 = 0;
  TIMSK2 |= _BV(OCIE2A); // enable output compare interrupt A on timer0

  pinMode(__inPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(__inPin), &ParkSensMon::startCountWidthOfPulse, RISING);
}

ISR(TIMER2_COMPA_vect) //5us
{
//  if (__enableCounting){
  if (__pulses == 9999) __pulses = 0;
  __pulses++;
// }
}

void ParkSensMon::startCountWidthOfPulse(void)
{
  __pulses = 0;
//  __enableCounting = 1;
  attachInterrupt(digitalPinToInterrupt(6), &ParkSensMon::countWidthOfPulse, FALLING);
}

void ParkSensMon::countWidthOfPulse(void)
{
if (__newData == 0){
//  __enableCounting=0;
  uint16_t __pulseWidth = __pulses * 5;
  __pulses = 0;
  if (__pulseWidth > 1000) //start
  {
    __bits = 0;
  } else {
    __data <<= 1;// 100us => 0
    if (__pulseWidth > 150) { //200us => 1
      __data |=  1;
    }
    __bits++; 
  }
  if (__bits == 33) { //we got all bits
    __newData = 1;
  }
}
  attachInterrupt(digitalPinToInterrupt(6), &ParkSensMon::startCountWidthOfPulse, RISING);
}

uint32_t ParkSensMon::read(void)
{
	__newData=0;
	return __data;
}


uint8_t ParkSensMon::available(void)
{
	return __newData;
}

uint8_t ParkSensMon::getDistance(uint8_t sensor)
{
	return ((__data>>(32-(sensor*8))) & 0xFF);
}

float ParkSensMon::getDistanceInMeters(uint8_t sensor)
{
        return (float)ParkSensMon::getDistance(sensor)/10;
}


void ParkSensMon::clearNewData(void)
{
	__newData=0;
}


