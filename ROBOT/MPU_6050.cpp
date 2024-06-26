#include "MPU_6050.h"

extern int measured_ACCEL_X = 0;

void init_MPU_6050()
{
  Wire.setClock(100000);              // predkosć połączenia I2C 100kHz
  Wire.begin();                       // rozpocznij połącznie i2C
  Wire.beginTransmission(MPU_6050);   // Rozpocznij I2C do MPU_6050
  Wire.write(0x6B);                   // rejestr 6B - power management
  Wire.write(0x00);                   // CLKsel = 8MGHz, sleep disabled = 0, 
  Wire.endTransmission(true);      
  _delay_ms(20);
}

void read_MPU_6050()
{
  Wire.beginTransmission(MPU_6050);   // Rozpocznij I2C do MPU_6050
  Wire.write(0x3B);                   // Zawartośc rejestru 0x3B czyli ACCEL_XOUT - poziom robota (roll)
  Wire.endTransmission(false);        
  Wire.requestFrom(MPU_6050, 2, true);  // odczytaj 2 rejestry ACCEL_XOUT_H i ACCEL_XOUT_L - 16 bit
  measured_ACCEL_X = (Wire.read() << 8 | Wire.read());  // zapisz wartość z bufora I2C
  float temp_acc_X = (measured_ACCEL_X /100.f - 27)/180.f;
  float temp_acc_x_rad = asin(temp_acc_X);
  measured_ACCEL_X = temp_acc_x_rad*180.f/3.14f;
}
