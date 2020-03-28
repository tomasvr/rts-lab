#ifndef __Schedule_c
#define __Schedule_c

/*
 * Preemptive, Time and event triggered scheduler.
 * Johan Lukkien, 12-01-2011
 *
 * Disclaimer: this code has only been tested for simple cases. There is a race
 *             condition between main program and interrupt routine regarding 
 *             going into sleep mode.
 * 
 * The scheduler is implemented by an array of tasks called Tasks, 
 * and a couple of functions. 
 *
 * Tasks must be declared as TRIGGERED, and pssibly PERIODIC.
 * Tasks are triggered (activated) by the timer (periodically or single shot) or 
 * by other tasks. The latter case is called event triggering. 
 *
 * One could regard the triggering of an event-based task the same as registering 
 * it. We discriminate between task registration and task triggering since the 
 * triggering may come from many different sources.
 *
 * Tasks execute according to their priorities and preempt each other if need be.
 *
 * The time resolution is TicksPS. We assume that all kernel functions execute in much
 * less than a single tick thus not causing timing inaccuracy. A proper overhead 
 * analysis needs to be done.
 *
 * Functions
 *   IntDisable():  disable interrupts and return previous status word;
 *   RestoreSW(sw): restore statusword
 *   Prio2Task(Prio): translate between priority and task pointer.
 *   InitTasks (): to be called upon the start of the system. Clears the
 *                 data structures.
 *   RegisterTask (): fill a Task structure. Operates under exclusion.
 *                    Activation of tasks is automatic for periodic
 *                    tasks; otherwise, specifying Flag TT will activate upon the next
 *                    timer interrupt. Activate() can also be called from other tasks.
 *                    Parameters have the following meaning.
 *         Phasing:  the number of time units to delay the first activation of a task
 *         Period:   period of a task
 *         TaskFunc: the function to call as the task body
 *         Prio:     priority (= index in Tasks)
 *         Flags:    TRIGGERED, PERIODIC
 *     Prio: task to activate
 *     Ticks: delay for activation. Ticks = 0 specifies immediate activation. 
 *   UnRegisterTask (): remove task from registration
 *         t: the task identifier
 *   Scheduler (): handle all tasks in the range MAXPRIO .. BusyPrio+1
 *   TimerIntrpt (): the timer interrupt routine. It counts down units for all 
 *                   TT marked tasks.
 *                   Whenever the count for a task reaches 0 the task get triggered.
 */

#include "Scheduler.h"
#include "Context.h"
#include "Led.h"

Task Tasks[NUMTASKS];           /* Lower indices: lower priorities           */
volatile uint8_t BusyPrio;      /* Current priority being served             */
volatile uint8_t Pending;       /* Is set when a task is pending */


void Scheduler (void);

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
  if (Pending && (sw & INTRPT_BIT)) Scheduler ();
    // r2 = sw
  asm volatile ("mov.w %0, r2\n\t" :: "r"(sw));
}  

Taskp Prio2Taskp (uint8_t Prio) __attribute__  ( (noinline) ); 
Taskp Prio2Taskp (uint8_t Prio)
{
  return (&Tasks[Prio]);
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
  BusyPrio = NUMTASKS;
  Pending = 0; 
}

/*
 * Register a task, TRIGGERED only, with flags.
 * Testing and filling in defaults is done.
 * Each priority level has at most one task.
 */

uint8_t RegisterTask (uint16_t Phasing, uint16_t Period, 
                      void (*TaskFunc) (void), uint8_t Prio, uint8_t Flags)
{
  uint8_t  rtc = E_SUCCESS;
  uint16_t sw;

  if (Prio>=NUMTASKS) return (E_BOUNDS); // out of bounds
  sw = IntDisable (); 
  Taskp t = &Tasks[Prio]; 
  if (t->Flags) rtc = E_BUSY; 
  else {
    t->Remaining = Phasing;
    t->Period    = Period; 
    t->Activated = t->Invoked = 0; 
    t->Taskf     = TaskFunc; 
    if (Period>0) Flags |= PERIODIC;
    if (Phasing>0 || Period>0) Flags |= TT;
    t->Flags = Flags | TRIGGERED;
  }
  RestoreSW (sw);
  return (rtc);
}

/*
 * UnRegisterTask(): remove task from registration
 * Note: this function is perhaps too simple and
 *       should take into account whether the 
 *       task is active or not.
 */

uint8_t UnRegisterTask (uint8_t t)
{
  if (t>=NUMTASKS) return (E_BOUNDS); // out of bounds
  Tasks[t].Flags = 0;
  return (E_SUCCESS);
}  

void Scheduler (void)
{ 
  SetLeds (BROWN, 0);

  uint8_t oldBP = BusyPrio; // Set BusyPrio as the priority of the currently running task (the one that was executing just before the scheduler is called)

  for(BusyPrio = 0; BusyPrio < oldBP; BusyPrio++)
  {
    Taskp CurTask = CurrentTask ();
    while (CurTask->Activated != CurTask->Invoked) {
      CurTask->Invoked++; 
      if (CurTask->Flags & TRIGGERED) {
        SetLeds (BROWN, 1);
        _EINT(); 
        CurTask->Taskf(); 
        _DINT();
        SetLeds (BROWN, 0);
      } else {
        CurTask->Activated = CurTask->Invoked;
      }
    }
  }

  SetLeds (BROWN, 1);
} 

interrupt (TIMERA0_VECTOR) TimerIntrpt (void)
{
  ContextSwitch();
  
  SetLeds (WHITE, 0);

  uint8_t i; 
  for(i = 0; i < NUMTASKS; i++)
  {
    Taskp t = &Tasks[i];
    if (t->Flags & TT) { // countdown
      if (t->Remaining-- == 0) {
        t->Activated++;
        t->Remaining = t->Period-1; 
        Pending |= i<BusyPrio;
      }
    }
  }

  SetLeds (WHITE, 1);

  if (Pending) 
  {
    Scheduler (); /* stay in interrupt context *
                   * interrupts disabled       */
    Pending = 0;
  }

  ResumeContext();
}

#endif
