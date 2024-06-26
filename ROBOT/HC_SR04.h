#ifndef HC_SR04_H
#define HC_SR04_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "Definitions.h"

extern int measuredDistance;      // ostatni zmierzony dystans 

void init_HCSR04(void);           // inicjalizuj moduł
void startDistanceMeasure(void);  // zainicjuj jeden pomiar

#endif //HC_SR_04_H
