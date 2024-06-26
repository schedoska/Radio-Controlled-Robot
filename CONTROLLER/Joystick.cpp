#include "Joystick.h"
#include <math.h>

JoyStickPos _joystickPos;

void read_joystick()
{
  ADMUX       &= ~(1 << MUX0);    // wybór wejścia konwertera ADC na PC2
  ADCSRA      |=  (1 << ADSC);    // Uruchamia jednorazowy odczyt
  while(ADCSRA & (1 << ADSC));    // Czekaj aż nit ADSC w ADCSRA opadnie -> koniec konwersji  
        _joystickPos.Speed = ADC; // Odczyt awrtości
  ADMUX       |= (1 << MUX0);     // wybór wejścia konwertera ADC na PC3
  ADCSRA      |= (1 << ADSC);     // Uruchamia jednorazowy odczyt
  while(ADCSRA & (1 << ADSC));    // Czekaj aż nit ADSC w ADCSRA opadnie -> koniec konwersji  
        _joystickPos.Steer = ADC; // Odczyt awrtości
}

void init_Joystick()
{
  ADMUX  |= (1 << REFS0);         // Napięcie odneisienia = AVcc
  ADCSRA |= (1 << ADEN ) |        // uruchomienie konwertera ADC
            (1 << ADPS2) |        // Przeskalownie clk/128 = 125kHz
            (1 << ADPS1) | 
            (1 << ADPS0);
  ADMUX |= (1 << MUX1);           
}

void getMotorValues(int *rightMotor, int *leftMotor)
{
  // Wyliczenie długosvci wektora sterowania 
  int lenght = sqrt(pow(_joystickPos.Steer - CENTERE_OFFSET, 2) + pow(_joystickPos.Speed - CENTERE_OFFSET, 2));
  
  if(_joystickPos.Steer - CENTERE_OFFSET < 0)
  {
    *rightMotor = lenght;
    *leftMotor = lenght * (float)_joystickPos.Steer / (float)CENTERE_OFFSET;
  }
  else
  {
    *leftMotor = lenght;
    *rightMotor = lenght * (float)(2 * CENTERE_OFFSET - _joystickPos.Steer) / (float)CENTERE_OFFSET;
  }
  *rightMotor *= FINAL_SCALER;  // Przeskalowanie finalne wartosci sliników 
  *leftMotor *= FINAL_SCALER;

  if(*rightMotor < IDLE_TOLERANCE) *rightMotor = 0; // Wyzerowanie wartośći sliników w przypadku pozycji 0 w określonej tolerancji
  if(*leftMotor < IDLE_TOLERANCE) *leftMotor = 0;

  if(_joystickPos.Speed < CENTERE_OFFSET)
  {
     *rightMotor *= -1;
     *leftMotor *= -1;
  }
}
