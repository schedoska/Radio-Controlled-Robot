#include "LIGHT_SENSOR.h"

int lightLevel = 0;

void read_lightSensor()
{
  //ADMUX       |= (1 << MUX0); 
  ADCSRA   |=  (1 << ADSC);     // Uruchamia jednorazowy odczyt ADC
  while(ADCSRA & (1 << ADSC));  // Czekaj aż nit ADSC w ADCSRA opadnie -> koniec konwersji  
  lightLevel = ADC;             // Odczyt awrtości
}

void init_lightSensor()
{
  ADMUX  |= (1 << REFS0);       // Napięcie odneisienia = AVcc
  ADCSRA |= (1 << ADEN ) |      // uruchomienie konwertera ADC
            (1 << ADPS2) |      // Przeskalownie clk/128 = 125kHz
            (1 << ADPS1) | 
            (1 << ADPS0);
}
