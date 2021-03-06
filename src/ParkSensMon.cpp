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

#define COMPARE_TIME 10
#define STARTPULSE 1500/COMPARE_TIME
#define LONGPULSE 150/COMPARE_TIME

//#define USE_TIMER0
//#define USE_TIMER1
#define USE_TIMER2
//#define USE_TIMER3
//#define USE_TIMER4
//#define USE_TIMER5

ParkSensMon::ParkSensMon(uint8_t inpin)
{
       __inPin = inpin;
}

ParkSensMon::~ParkSensMon(){}

#ifdef USE_TIMER0
void ParkSensMon::setTimer(void){
  // @ 0.5us ticks
  TCCR0A = 0x00; // Normal port operation, OC0 disconnected
  TCCR0B = 0x00; // Normal port operation, OC0 disconnected
  TCCR0A |= _BV(WGM01); // CTC mode
  TCCR0B |= _BV(CS01);// prescaler = 8 -> 1 timer clock tick is 0.5us long @ 16Mhz
  OCR0A = COMPARE_TIME*2;//run compare
  TCNT0 = 0;
  TIMSK0 |= _BV(OCIE0A); // enable output compare interrupt A on timer0
}
#define __TIMERX_COMPA_vect TIMER0_COMPA_vect
#undef USE_TIMER1
#undef USE_TIMER2
#undef USE_TIMER3
#undef USE_TIMER4
#undef USE_TIMER5
#endif

#ifdef USE_TIMER1
void ParkSensMon::setTimer(void){
  // @ 0.5us ticks
  TCCR1A = 0x00; // Normal port operation, OC0 disconnected
  TCCR1B = 0x00; // Normal port operation, OC0 disconnected
  TCCR1B |= _BV(WGM12); // CTC mode
  TCCR1B |= _BV(CS11);// prescaler = 8 -> 1 timer clock tick is 0.5us long @ 16Mhz
  OCR1A = COMPARE_TIME*2;//run compare
  TCNT1 = 0;
  TIMSK1 |= _BV(OCIE1A); // enable output compare interrupt A on timer0
}
#define __TIMERX_COMPA_vect TIMER1_COMPA_vect
#undef USE_TIMER2
#undef USE_TIMER3
#undef USE_TIMER4
#undef USE_TIMER5
#endif

#ifdef USE_TIMER2
void ParkSensMon::setTimer(void){
  // @ 0.5us ticks
  TCCR2A = 0x00; // Normal port operation, OC0 disconnected
  TCCR2B = 0x00; // Normal port operation, OC0 disconnected
  TCCR2A |= _BV(WGM21); // CTC mode
  TCCR2B |= _BV(CS21);// prescaler = 8 -> 1 timer clock tick is 0.5us long @ 16Mhz
  OCR2A = COMPARE_TIME*2;//run compare
  TCNT2 = 0;
  TIMSK2 |= _BV(OCIE2A); // enable output compare interrupt A on timer0
}
#define __TIMERX_COMPA_vect TIMER2_COMPA_vect
#undef USE_TIMER3
#undef USE_TIMER4
#undef USE_TIMER5
#endif

#ifdef USE_TIMER3
void ParkSensMon::setTimer(void){
  // @ 0.5us ticks
  TCCR3A = 0x00; // Normal port operation, OC0 disconnected
  TCCR3B = 0x00; // Normal port operation, OC0 disconnected
  TCCR3B |= _BV(WGM32); // CTC mode
  TCCR3B |= _BV(CS31);// prescaler = 8 -> 1 timer clock tick is 0.5us long @ 16Mhz
  OCR3A = COMPARE_TIME*2;//run compare
  TCNT3 = 0;
  TIMSK3 |= _BV(OCIE3A); // enable output compare interrupt A on timer0
}
#define __TIMERX_COMPA_vect TIMER3_COMPA_vect
#undef USE_TIMER4
#undef USE_TIMER5
#endif

#ifdef USE_TIMER4
void ParkSensMon::setTimer(void){
  // @ 0.5us ticks
  TCCR4A = 0x00; // Normal port operation, OC0 disconnected
  TCCR4B = 0x00; // Normal port operation, OC0 disconnected
  TCCR4B |= _BV(WGM42); // CTC mode
  TCCR4B |= _BV(CS41);// prescaler = 8 -> 1 timer clock tick is 0.5us long @ 16Mhz
  OCR4A = COMPARE_TIME*2;//run compare
  TCNT4 = 0;
  TIMSK4 |= _BV(OCIE4A); // enable output compare interrupt A on timer0
}
#define __TIMERX_COMPA_vect TIMER4_COMPA_vect
#undef USE_TIMER5
#endif

#ifdef USE_TIMER5
void ParkSensMon::setTimer(void){
  // @ 0.5us ticks
  TCCR5A = 0x00; // Normal port operation, OC0 disconnected
  TCCR5B = 0x00; // Normal port operation, OC0 disconnected
  TCCR5B |= _BV(WGM52); // CTC mode
  TCCR5B |= _BV(CS51);// prescaler = 8 -> 1 timer clock tick is 0.5us long @ 16Mhz
  OCR5A = COMPARE_TIME*2;//run compare
  TCNT5 = 0;
  TIMSK5 |= _BV(OCIE5A); // enable output compare interrupt A on timer0
}
#define __TIMERX_COMPA_vect TIMER5_COMPA_vect
#endif

void ParkSensMon::begin()
{
  setTimer();
  pinMode(__inPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(__inPin), &ParkSensMon::startCountWidthOfPulse, RISING);
}

ISR(__TIMERX_COMPA_vect) //5us
{
//  if (__enableCounting){
  __pulses++;
// }
}

void ParkSensMon::startCountWidthOfPulse(void)
{
  __pulses = 0;
//  __enableCounting = 1;
  attachInterrupt(digitalPinToInterrupt(__inPin), &ParkSensMon::countWidthOfPulse, FALLING);
}

void ParkSensMon::countWidthOfPulse(void)
{
if (__newData == 0) {
//  __enableCounting=0;
  uint8_t __pulseWidth = __pulses;
  __pulses = 0;
  if (__pulseWidth > STARTPULSE) //start
  {
    __bits = 0;
  }
  else
  {
    __data[(__bits-1)/8] <<= 1;// 100us => 0
    if (__pulseWidth > LONGPULSE) { //200us => 1
      __data[(__bits-1)/8] |=  1;
    }
    __bits++; 
  }
  if (__bits == 33) { //we got all bits
    __newData = 1;
  }
}
  attachInterrupt(digitalPinToInterrupt(__inPin), &ParkSensMon::startCountWidthOfPulse, RISING);
}

uint8_t ParkSensMon::available(void)
{
	return __newData;
}

uint8_t ParkSensMon::objectDetected(void)
{
	if (__newData && (__data[0]<0xFF || __data[1]<0xFF || __data[2]<0xFF || __data[3]<0xFF)) return true;
	return false;
}

uint8_t ParkSensMon::getDistance(uint8_t sensor)
{
	switch(sensor)
	{
		case 1:	return __data[0];
		case 2:	return __data[3];
		case 3:	return __data[2];
		case 4:	return __data[1];
	}
}

float ParkSensMon::getDistanceInMeters(uint8_t sensor)
{
	return (float)ParkSensMon::getDistance(sensor)/10;
}


void ParkSensMon::clearNewData(void)
{
	__newData=0;
}


