#include <io.h>
#include <iomacros.h>

#include "TimeTracking.h"

/*
 * Timetracking interface, used to track the amount of time spent on certain intervals in code
 *
 * Boris Blokland, 8-2-2019
 */

static uint16_t StartTimes[TT_MAX];
static uint16_t Totals[TT_MAX];
static uint8_t JobsExecuted;

void StartTracking(uint8_t index)
{
    StartTimes[index] = TAR;
}

void StopTracking(uint8_t index)
{
    Totals[index] += TAR - StartTimes[index];
}

void AddJobExecution()
{
    JobsExecuted++;
}

void PrintResults()
{
    P1OUT = Totals[TT_TIMER_INTERRUPT] & 0xFF;
    P2OUT = Totals[TT_TIMER_INTERRUPT] >> 8;

    P3OUT = Totals[TT_SCHEDULER] & 0xFF;
    P4OUT = Totals[TT_SCHEDULER] >> 8;

    P6OUT = JobsExecuted;
}