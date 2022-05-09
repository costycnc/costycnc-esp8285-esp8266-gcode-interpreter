/****************************************************************************************************************************
  TimerInterruptTest.ino
  For ESP8266 boards
  Written by Khoi Hoang

  Built by Khoi Hoang https://github.com/khoih-prog/ESP8266TimerInterrupt
  Licensed under MIT license

  The ESP8266 timers are badly designed, using only 23-bit counter along with maximum 256 prescaler. They're only better than UNO / Mega.
  The ESP8266 has two hardware timers, but timer0 has been used for WiFi and it's not advisable to use. Only timer1 is available.
  The timer1's 23-bit counter terribly can count only up to 8,388,607. So the timer1 maximum interval is very short.
  Using 256 prescaler, maximum timer1 interval is only 26.843542 seconds !!!

  Now with these new 16 ISR-based timers, the maximum interval is practically unlimited (limited only by unsigned long miliseconds)
  The accuracy is nearly perfect compared to software timers. The most important feature is they're ISR-based timers
  Therefore, their executions are not blocked by bad-behaving functions / tasks.
  This important feature is absolutely necessary for mission-critical tasks.
*****************************************************************************************************************************/
/* Notes:
   Special design is necessary to share data between interrupt code and the rest of your program.
   Variables usually need to be "volatile" types. Volatile tells the compiler to avoid optimizations that assume
   variable can not spontaneously change. Because your function may change variables while your program is using them,
   the compiler needs this hint. But volatile alone is often not enough.

   When accessing shared variables, usually interrupts must be disabled. Even with volatile,
   if the interrupt changes a multi-byte variable between a sequence of instructions, it can be read incorrectly.
   If your data is multiple variables, such as an array and a count, usually interrupts need to be disabled
   or the entire sequence of your code which accesses the data.
*/

#if !defined(ESP8266)
  #error This code is designed to run on ESP8266 and ESP8266-based boards! Please check your Tools->Board setting.
#endif

// These define's must be placed at the beginning before #include "ESP8266TimerInterrupt.h"
// _TIMERINTERRUPT_LOGLEVEL_ from 0 to 4
// Don't define _TIMERINTERRUPT_LOGLEVEL_ > 0. Only for special ISR debugging only. Can hang the system.
#define TIMER_INTERRUPT_DEBUG         1
#define _TIMERINTERRUPT_LOGLEVEL_     1

// Select a Timer Clock
#define USING_TIM_DIV1                false           // for shortest and most accurate timer
#define USING_TIM_DIV16               false           // for medium time and medium accurate timer
#define USING_TIM_DIV256              true            // for longest timer but least accurate. Default

#include "ESP8266TimerInterrupt.h"

#define BUILTIN_LED     2       // Pin D4 mapped to pin GPIO2/TXD1 of ESP8266, NodeMCU and WeMoS, control on-board LED

volatile bool statusLed = false;
volatile uint32_t lastMillis = 0;
volatile uint32_t costy = 0;

#define TIMER_INTERVAL_MS       1

// Init ESP8266 timer 1
ESP8266Timer ITimer;

//=======================================================================
void IRAM_ATTR TimerHandler()
{
  static bool started = false;

  if (!started)
  {
    started = true;
    pinMode(2, OUTPUT);
    pinMode(0, OUTPUT);
    pinMode(4, OUTPUT);
    pinMode(5, OUTPUT);
  }
if (costy==1)
{
  digitalWrite(2, 1);  //Toggle LED Pin
  digitalWrite(0, 0);  //Toggle LED Pin
  digitalWrite(4, 0);  //Toggle LED Pin
  digitalWrite(5, 0);  //Toggle LED Pin
}  
if (costy==2)
{
  digitalWrite(2, 1);  //Toggle LED Pin
  digitalWrite(0, 1);  //Toggle LED Pin
  digitalWrite(4, 0);  //Toggle LED Pin
  digitalWrite(5, 0);  //Toggle LED Pin
}  
if (costy==3)
{
  digitalWrite(2, 0);  //Toggle LED Pin
  digitalWrite(0, 1);  //Toggle LED Pin
  digitalWrite(4, 0);  //Toggle LED Pin
  digitalWrite(5, 0);  //Toggle LED Pin
}  
if (costy==4)
{
  digitalWrite(2, 0);  //Toggle LED Pin
  digitalWrite(0, 1);  //Toggle LED Pin
  digitalWrite(4, 1);  //Toggle LED Pin
  digitalWrite(5, 0);  //Toggle LED Pin
}  
if (costy==5)
{
  digitalWrite(2, 0);  //Toggle LED Pin
  digitalWrite(0, 0);  //Toggle LED Pin
  digitalWrite(4, 1);  //Toggle LED Pin
  digitalWrite(5, 0);  //Toggle LED Pin
}  
if (costy==6)
{
  digitalWrite(2, 0);  //Toggle LED Pin
  digitalWrite(0, 0);  //Toggle LED Pin
  digitalWrite(4, 1);  //Toggle LED Pin
  digitalWrite(5, 1);  //Toggle LED Pin
}  
if (costy==7)
{
  digitalWrite(2, 0);  //Toggle LED Pin
  digitalWrite(0, 0);  //Toggle LED Pin
  digitalWrite(4, 0);  //Toggle LED Pin
  digitalWrite(5, 1);  //Toggle LED Pin
}  
if (costy==8)
{
  digitalWrite(2, 1);  //Toggle LED Pin
  digitalWrite(0, 0);  //Toggle LED Pin
  digitalWrite(4, 0);  //Toggle LED Pin
  digitalWrite(5, 1);  //Toggle LED Pin
  costy=0;
}  
costy++;
  //statusLed = !statusLed;

#if (TIMER_INTERRUPT_DEBUG > 0)

  lastMillis = millis();
#endif
}
//=======================================================================
//                               Setup
//=======================================================================
void setup()
{
  Serial.begin(115200);
  while (!Serial);

  delay(300);



  // Interval in microsecs
  if (ITimer.attachInterruptInterval(TIMER_INTERVAL_MS * 500, TimerHandler))
  {
    lastMillis = millis();

  }



}
//=======================================================================
//                MAIN LOOP
//=======================================================================
void loop()
{
}
