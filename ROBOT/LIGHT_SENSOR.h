#ifndef LIGHT_SENSOR_H
#define LIGHT_SENSOR_H
#include <avr/io.h>

extern int lightLevel;    //ostatnia zmierzona wrtośc natęzenia

void init_lightSensor();  //inicjalizuj czujnik natezenia
void read_lightSensor();  //odczytaj jednorazowo wartość

#endif //LIGHT_SENSOR_H
