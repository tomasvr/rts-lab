#ifndef __Clock_h
#define __Clock_h

#define TicksPS    1024 /* # clock ticks per second       */

#define EnterLowPowerMode3() _BIC_SR    (LPM3_bits)
#define ExitLowPowerMode3()  _BIC_SR_IRQ(LPM3_bits)

void InitClock (void);  /* to be called upon system start */

#endif
