/*
 * This file is controlled by constants
 * 
 * For link against:
 *   a scheduler with threads:   #define THREADS 
 *   a non-preemptive scheduler: #define NONPRE
 *   the simplest non-preemtpive scheduler: 
 *                               #define NONPREBASIC
 *
 * If semaphores have been defined:
 *                               #define SEMA
 * If an UART has been added:    #define UART
 */

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

void BlinkYellow (void)
{
  SetLeds (YELLOW, 1);
  CountDelay(10000);
  SetLeds (YELLOW, 0);
}

void BlinkGreen (void)
{
  SetLeds (GREEN, 1);
  CountDelay(10000);
  SetLeds (GREEN, 0);
}

void BlinkRed (void)
{
  SetLeds (RED, 1);
  CountDelay(20000);
  SetLeds (RED, 0);
}

int main(void) 
{ 
  WDTCTL = WDTPW + WDTHOLD;       // Stop watchdog timer 
  
  InitTasks (); 
  InitClock ();
  InitLeds (RED | GREEN | YELLOW);

  RegisterTask (0, 512, BlinkYellow, YELLOWBLINKPRIO, 0);
  RegisterTask (0, 1024, BlinkGreen, GREENBLINKPRIO, 0);
  RegisterTask (0, 1536, BlinkRed, REDBLINKPRIO, 0);

  _EINT();                // Enable interrupts

  while (1)
  {
    EnterLowPowerMode3(); // idle task, set to low power mode 3
                          // (Aclock still working)
  } 
  return (0);
}
