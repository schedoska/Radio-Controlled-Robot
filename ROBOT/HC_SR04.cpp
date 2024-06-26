#include "HC_SR04.h"

extern uint8_t isMeasurementStarted = FALSE;
extern int measuredDistance = 0;

void init_HCSR04(void)
{
  DDRD |= (1<<TRIGGER_PIN); // PIN 3 PORT D na wyjście 
  //DDRD &= ~(1<<INTERRUPT_PIN);
  EICRA |= (1<<ISC00); // Wykrywanie zmiany stanu pinu INT0
  EIMSK |= (1<<INT0);  // Uruchomienie przerwania na zmiane stanu
  TCCR2B |= (1<<CS22) | (1<<CS20); // Przeskalowanie: clk/1024
  sei();
}

void startDistanceMeasure(void)
{
  PORTD |= (1<<TRIGGER_PIN);  // Wyjście trigger na HIGH przez 10us
  _delay_us(10);
  PORTD &= ~(1<<TRIGGER_PIN); // Wyjście trigger na HIGH
}

ISR(INT0_vect)
{
  if(isMeasurementStarted == FALSE)
  {
    isMeasurementStarted = TRUE;
    TCNT2 = 0; // Wyzerowanie stanu timera
  }
  else
  {
    isMeasurementStarted = FALSE;
    measuredDistance = TCNT2; // Odczyt stanu timera
  }
}
