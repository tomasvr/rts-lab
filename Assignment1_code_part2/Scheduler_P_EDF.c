#include "Scheduler.h"
#include "Led.h"

static void ExecuteTask (Taskp t)
{
  /* insert code */
  t->Taskf(t->ExecutionTime); // execute task
  /* insert code */
}

void Scheduler_P_EDF (Task Tasks[])
{ 
  /* insert code */
  /* Use ExecuteTask function to execute a task */
  /* insert code */
}
