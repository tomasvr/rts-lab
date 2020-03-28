#ifndef __TimeTracking_h
#define __TimeTracking_h

/*
 * Timetracking interface
 *
 * Boris Blokland, 8-2-2019
 */

enum TimeTrackingIndex {
    TT_TIMER_INTERRUPT,
    TT_SCHEDULER,

    TT_MAX
};

void StartTracking(uint8_t index);
void StopTracking(uint8_t index);
void AddJobExecution();
void PrintResults();

#endif
