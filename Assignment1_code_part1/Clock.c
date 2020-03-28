#ifndef __Clock_c
#define __Clock_c

/*
 * Clock driver
 *
 * Clock A is used to generate 1024 interrupts per second.
 * Usage of clock A admits deep sleep mode.
 * 
 * Call the InitClock function after program start.
 *
 * JL, 22-12-2010
 */

#include <io.h>
#include <iomacros.h>
#include "Clock.h"

#define ACLOCK    32768  /* rate of the Aclock                               */

void InitClock (void)
{
	TACTL  = TASSEL0 + TACLR; /* ACLK, clear TAR                               */
	CCR0   = ACLOCK/TicksPS;  /* #ticks per interrupt:1024 Hz in this case     */
	CCTL0  = CCIE;            /* CCR0 interrupt enabled                        */
	TACTL |= MC0;		    /* Start Timer_a in upmode                       */
}

#endif
