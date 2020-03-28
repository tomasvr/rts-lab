#ifndef __Scheduler_h
#define __Scheduler_h

#include <io.h>
#include <signal.h>
#include <sys/inttypes.h>
#include <iomacros.h>
#include <stdlib.h>
#include "ErrorCodes.h"

#define NUMTASKS    7  /* # tasks admitted                                 */

#define DIRECT      1   /* task flag: execute uninterruptable, when ready   */
#define PERIODIC    2   /* general flag: trigger by timer interrupt         */
#define TT          4   /* general flag: waiting to be Time Triggered       */
#define ACTIVE      8   /* task state: running, or interrupted while running
                         *             not ACTIVE is called PASSIVE         */
#define BLOCKED    16   /* task state: blocked on synchronization primitive */
#define THREAD     32   /* type: THREAD (context of task retained)          */
#define TRIGGERED  64   /* type: TRIGGERED: context created upon triggering */
#define FPDS       128  /* type: FPDS: preemptive only at preemption points */

#define THRBLKSIZ  256  /* thread context size: 256 bytes                   */
#define BUSY_EXEC  512  /* task flag: this task has started executing       */
#define GLOBMEMSIZ 1024 /* space reserved for global stack                  */

typedef struct Task *Taskp;    /* task pointer                              */

typedef struct Task {
  volatile uint16_t NextRelease;  /* absolute time of next execution            */
  volatile uint16_t NextPendingDeadline; /* the next, unmet deadline            */
           uint16_t Period;       /* activation period                          */
  volatile uint8_t Activated;     /* # pending activations (8 bit wrap around)  */
           uint8_t Invoked;       /* # activations served (8 bit wrap around)   */
  volatile uint16_t Flags;        /* task properties                            */
           uint16_t ExecutionTime;/* Execution time of the task */ 
  void (*Taskf) (uint16_t);       /* function to be called as task body         */
  uint8_t Prio;                   /* priority, field needed for reverse lookup  */
} Task;
  
#define NULLTASK (Taskp) 0

#define INTRPT_BIT  0x8        /* Interrupt enable in PSW                   */

uint16_t IntDisable (void);
void     RestoreSW (uint16_t sw);

void    InitTasks (void);
uint8_t RegisterTask (uint16_t Phasing, uint16_t Period, uint16_t ExecutionTime,
                      void (*TaskFunc) (uint16_t), uint8_t Prio, uint8_t Flags);
uint8_t UnRegisterTask (uint8_t Prio);

#define P_FP

#if defined (P_RM)
#define CALL_SCHEDULER Scheduler_P_RM(Tasks);
void Scheduler_P_RM (Task Tasks[]);

#elif defined (P_FP)
#define CALL_SCHEDULER Scheduler_P_FP(Tasks);
void Scheduler_P_FP (Task Tasks[]);

#elif defined (P_EDF)
#define CALL_SCHEDULER Scheduler_P_EDF(Tasks);
void Scheduler_P_EDF (Task Tasks[]);

#elif defined (CW_EDF)
#define CALL_SCHEDULER Scheduler_CW_EDF(Tasks);
void Scheduler_CW_EDF (Task Tasks[]);

#endif


#endif
