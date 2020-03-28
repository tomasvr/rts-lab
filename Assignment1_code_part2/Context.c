#ifndef __Context_c
#define __Context_c

/*
 * Context switch
 *
 * Context switches are used in pre-emptive scheduling to store current state, and resume it when neccessary
 *
 * Boris Blokland, 15-01-2019
 */

#include <io.h>
#include <iomacros.h>
#include "Scheduler.h"
#include "Led.h"
#include "Context.h"

static uint8_t Contexts[NUMTASKS];
static uint8_t CurrentContext = 0;

void ContextSwitch()
{
	Contexts[CurrentContext] = LEDPIN; // store current context
	CurrentContext++;
	// TODO set this to 0xFF for leds
	LEDPIN = 0; // disable all leds
}

void ResumeContext()
{
	CurrentContext--;
	LEDPIN = Contexts[CurrentContext]; // resume context
}

#endif
