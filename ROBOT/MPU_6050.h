#include <Wire.h>
#include <util/delay.h>

#define MPU_6050 0x68 //Adres urządzenia I2C

extern int measured_ACCEL_X;  //Ostatnia zmierzona wartość poziomu robota

void init_MPU_6050(); //inicjalizuj moduł akcelerometru
void read_MPU_6050(); //odczytaj wartości rejetrów akcelerometru (poziom)
