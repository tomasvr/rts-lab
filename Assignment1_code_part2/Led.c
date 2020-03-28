#ifndef __Led_c
#define __Led_c

/*
 * Led driver, assumes all leds on same output pin
 *
 * Leds are mapped to a bit of a GPIO pin, dependent on the MSP430 type
 * Initialize: set direction of GPIO (1 is output, 0 is input)
 *             set led off (1 is off, 0 is on)
 * 
 * JL, 22-12-2010
 */

#include <io.h>
#include <iomacros.h>
#include "Led.h"

void InitLeds (uint8_t Colors)
{
  LEDDIR |= Colors;
  LEDPIN = 0;
}

void SetLeds (uint8_t Colors, uint8_t On)
{
  if (On) LEDPIN |= Colors;
  else    LEDPIN &= ~Colors;
}

void ToggleLeds (uint8_t Colors)
{
  LEDPIN ^= Colors;
}

#endif
