#include "Scheduler.h"
#include "Led.h"
#include "TimeTracking.h"

static void ExecuteTask (Taskp t)
{
  /* ----------------------- INSERT CODE HERE ----------------------- */
  t->Flags |= BUSY_EXEC;
  _EINT(); 
  t->Taskf(t->ExecutionTime); // execute task
  _DINT();
  t->Invoked++;
  t->Flags &= ~(BUSY_EXEC);
  /* ---------------------------------------------------------------- */
}

void Scheduler_P_FP (Task Tasks[])
{ 
  /* ----------------------- INSERT CODE HERE ----------------------- */
  StartTracking(TT_SCHEDULER);
  
  SetLeds (WHITE, 1);

  uint8_t taskFound = 0;

  /* Loop as long as a pending task is found */
  do {
    SetLeds (PURPLE, 1);
    taskFound = 0;

    /* Loop over all tasks until ready task is found */
    uint8_t k=NUMTASKS-1; 
    do { 
      Taskp t = &Tasks[k];

      /* If the task needs to be executed.. */
      if (t->Activated > t->Invoked ) {
          /* If the task was not already executing */
          if ( !(t->Flags & BUSY_EXEC) ) {            
            taskFound = 1;
            StopTracking(TT_SCHEDULER);
            PrintResults();
            ExecuteTask(t);  
            StartTracking(TT_SCHEDULER);
            AddJobExecution();
          }
          break;
      }
    } while (k--);
    SetLeds (PURPLE, 0);
  } while (taskFound == 1);
  
  SetLeds (WHITE, 0);
  
  StopTracking(TT_SCHEDULER);
  PrintResults();

  /* End of example*/

  /* ---------------------------------------------------------------- */
}
