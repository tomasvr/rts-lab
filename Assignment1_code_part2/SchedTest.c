#include <io.h>
#include <iomacros.h>

#include "Led.h"
#include "Clock.h"
#include "Scheduler.h"

#define YELLOWBLINKPRIO 0
#define GREENBLINKPRIO  1
#define REDBLINKPRIO    2

void CountDelay (volatile uint16_t cnt)
{
  while (cnt--);
}

void CountRealtimeDelay (uint16_t Ticks)
{
  uint16_t TicksPassed = 0;
  uint16_t OldTAR;

  Ticks *= 4; // multiply by 4, to get ms approximately!

  OldTAR = TAR; // store current value of TAR
  while (TicksPassed < Ticks) // continue until all ticks have passed
  {
    if (OldTAR != TAR) // a new value of TAR exists, thus a tick has occured
    {
      TicksPassed++; // increment ticks counter
      OldTAR = TAR; // store new value of TAR as old value
    }
  }
}

void BlinkYellow (uint16_t ExecutionTime)
{
  SetLeds (YELLOW, 1);
  CountRealtimeDelay(ExecutionTime);
  SetLeds (YELLOW, 0);
}

void BlinkGreen (uint16_t ExecutionTime)
{
  SetLeds (GREEN, 1);
  CountRealtimeDelay(ExecutionTime);
  SetLeds (GREEN, 0);
}

void BlinkRed (uint16_t ExecutionTime)
{
  SetLeds (RED, 1);
  CountRealtimeDelay(ExecutionTime);
  SetLeds (RED, 0);
}

int main(void) 
{ 
  WDTCTL = WDTPW + WDTHOLD;       // Stop watchdog timer 
  
  InitClock ();

  InitLeds (RED | GREEN | YELLOW | WHITE | BROWN | PURPLE);

  //InitTasks(); // this wasn't called by default


#define TstSweep

#ifdef Tst1
  // note: there 1024 ticks in second. This means signals in tkwave will be a little shorter
  RegisterTask (0, 1024, 350, BlinkYellow, YELLOWBLINKPRIO, 0); 
  RegisterTask (0, 512, 140, BlinkGreen, GREENBLINKPRIO, 0);
  RegisterTask (0, 256, 30, BlinkRed, REDBLINKPRIO, 0);
#endif

#ifdef Tst2
  // note: there 1024 ticks in second
  RegisterTask (0, 256, 30, BlinkYellow, YELLOWBLINKPRIO, 0); 
  RegisterTask (0, 512, 150, BlinkGreen, GREENBLINKPRIO, 0);
  RegisterTask (0, 1024, 350, BlinkRed, REDBLINKPRIO, 0);
#endif

#ifdef Tst3
  // note: there 1024 ticks in second
  RegisterTask (0, 256, 30, BlinkYellow, YELLOWBLINKPRIO, 0); 
  RegisterTask (0, 512, 350, BlinkGreen, GREENBLINKPRIO, 0);
  RegisterTask (0, 1024, 150, BlinkRed, REDBLINKPRIO, 0);
#endif

#ifdef TstSweep
  // note: there 1024 ticks in second
  RegisterTask (0, 1024, 350, BlinkYellow, YELLOWBLINKPRIO, 0); 
  RegisterTask (0, 512, 140, BlinkGreen, GREENBLINKPRIO, 0);
  RegisterTask (0, 256, 30, BlinkRed, REDBLINKPRIO, 0);
  RegisterTask (0, 128, 10, BlinkRed, 3, 0);
  RegisterTask (0, 256, 2, BlinkRed, 4, 0);

  RegisterTask (0, 1024, 10, BlinkYellow, 5, 0); 
  RegisterTask (0, 512, 10, BlinkGreen, 6, 0);
  RegisterTask (0, 256, 5, BlinkRed, 7, 0);
  RegisterTask (0, 128, 5, BlinkRed, 8, 0);
  RegisterTask (0, 256, 2, BlinkRed, 9, 0);
#endif


  _EINT();                        // Enable interrupts

  while(1) {
    EnterLowPowerMode3(); // idle task, set to low power mode 3 

  }
  return (0);
}
