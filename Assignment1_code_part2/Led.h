#ifndef __Led_h
#define __Led_h

/*
 * Led interface
 *
 * JL, 22-12-2010
 */

#include <io.h>
#include <iomacros.h>

#define PURPLE (uint8_t) 0x01
#define BROWN  (uint8_t) 0x02
#define WHITE  (uint8_t) 0x04

#define RED    (uint8_t) 0x10
#define GREEN  (uint8_t) 0x20
//#define BLUE   (uint8_t) 0x20
#define YELLOW (uint8_t) 0x40 // should be blue actually

#define ON  (uint8_t) 1
#define OFF (uint8_t) 0

#define LEDDIR P5DIR
#define LEDPIN P5OUT

void InitLeds   (uint8_t Colors);
void SetLeds    (uint8_t Colors, uint8_t On);
void ToggleLeds (uint8_t Colors);

#endif
