#ifndef __Schedule_c
#define __Schedule_c

#include "Clock.h"
#include "Scheduler.h"
#include "Led.h"
#include "Context.h"

#include "TimeTracking.h"

Task Tasks[NUMTASKS];           /* Lower indices: lower priorities           */
uint16_t NextInterruptTime;     /* Timestamp at which next interrupt should occur */

uint16_t IntDisable (void)
{
  uint16_t sw;
    // sw = r2
  asm volatile ("mov.w r2, %0\n\t" : "=r"(sw));
  _DINT();
  return (sw);
}

void RestoreSW (uint16_t sw)
{
    // r2 = sw
  asm volatile ("mov.w %0, r2\n\t" :: "r"(sw));
}  

/* 
 * Initialize and clear task structures.
 * Should be called with interrupt disabled.
 * The clock must be started elsewhere.
 */

void InitTasks (void)
{			
  uint8_t i=NUMTASKS-1; 
  do { 
    Taskp t = &Tasks[i];
    t->Flags = t->Activated = t->Invoked = 0;
  } while (i--);
}

/*
 * Register a task, TRIGGERED only, with flags.
 * Testing and filling in defaults is done.
 * Each priority level has at most one task.
 */

uint8_t RegisterTask (uint16_t Phasing, uint16_t Period, uint16_t ExecutionTime,
                      void (*TaskFunc) (uint16_t), uint8_t Prio, uint8_t Flags)
{
  uint8_t  rtc = E_SUCCESS;
  uint16_t sw;

  Phasing *= 4; // correct to get ms approximately
  Period *=4; // correct to get ms approximately

  if (Prio>=NUMTASKS) return (E_BOUNDS); // out of bounds
  if (Period == 0) return (E_WRONGPAR);
  sw = IntDisable (); 
  Taskp t = &Tasks[Prio]; 
  if (t->Flags) rtc = E_BUSY; 
  else {
    t->NextRelease = 0 + Phasing;
    t->Period    = Period; 
    t->NextPendingDeadline = t->NextRelease + Period;
    t->Activated = t->Invoked = 0; 
    t->Taskf     = TaskFunc; 
    t->ExecutionTime = ExecutionTime;
    t->Flags     = Flags | TRIGGERED;
    //t->TimeLeft = 0;

  }
  RestoreSW (sw);
  return (rtc);
}

uint8_t UnRegisterTask (uint8_t t)
{
  Tasks[t].Flags = 0;
  return (E_SUCCESS);
}  

static void DetermineNextInterruptTime (CandidateValue)
{
  if (CandidateValue < NextInterruptTime)
  {
    NextInterruptTime = CandidateValue;
  }
}

/*
  The timer interrupt is configured in continious mode with compare mode interrupts enabled.
  The source for Timer_A is ACLK/8 (32768 / 8 = 4096 Hz). Thus every 1/4096 second, the TAR register is incremented.
  When the TAR register reaches TACCR0, an interrupt is generated. Note that TAR will NOT be reset!
  It continues incrementing until it reaches it's max value (16-bit timer -> 65535), it will continue counting from 0.

  Relevant registers:
    TAR     Current value of Timer_A. 
    TACCR0  Compare register, when TAR reached TACCR0, an interrupt is generated

  Your job is to set a new value of TACCR0 every timer interrupt, to get the next interrupt

  Use the Task data structure defined in 'Scheduler.h' to store information which can be used by the scheduler.

*/

interrupt (TIMERA0_VECTOR) TimerIntrpt (void)
{
  StartTracking(TT_TIMER_INTERRUPT);

  ContextSwitch();


  /* Loop over all tasks released at current timestamp and update next release time */
  uint8_t i=NUMTASKS-1; 
  do { 
    Taskp t = &Tasks[i];
    if (t->NextRelease <= NextInterruptTime) { // here NextIntrpTime should be the current time
      t->NextRelease += t->Period; // set next release time
      t->Activated++;
    }
  } while (i--);

  /* Select highest prio task as candidate for closest next release time */
  uint16_t minRelease = (&Tasks[NUMTASKS-1])->NextRelease; 

  /* Loop over all tasks and remember closest next release time */
  uint8_t j=NUMTASKS-1; 
  do { 
    Taskp t = &Tasks[j];
    if (t->NextRelease < minRelease) {
      minRelease = t->NextRelease;
    }
  } while (j--);
  
  /* Update global variable NextInterruptTime and Clock trigger */
  NextInterruptTime = minRelease; //% 65536; //TODO: check if mod is correct // not necesarry i think as t->NextReleas is max 16 bit
  TACCR0 = NextInterruptTime;

  StopTracking(TT_TIMER_INTERRUPT);

  CALL_SCHEDULER;

  ResumeContext();

}

#endif

