#ifndef DC_MOTOR_H
#define DC_MOTOR_H

#include <avr/io.h>
#include "Definitions.h"
#define MOTOR_SPEED_MAX 1020

void init_DCmotors(void);   //inicjalizuje silniki 
void set_RightMotorSpeed(int motorRight); // ustaw predkość prawego silnika
void set_LeftMotorSpeed(int motorLeft);   // ustaw predkosc lewego silnika

#endif //DC_MOTOR_H
