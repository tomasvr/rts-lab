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
	TACTL  = TASSEL0 + TACLR + ID0 + ID1 + TAIE; /* ACLK, clear TAR, set clock divider at 8 */
	TACCR0   = 0;  /* start first interrupt right away     */
	TACCTL0  = CCIE;            /* CCR0 interrupt enabled                        */
	TACTL |= MC1;		    /* Start Timer_a in continious mode */  
}

#endif
