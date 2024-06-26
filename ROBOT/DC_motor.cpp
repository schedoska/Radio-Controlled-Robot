#include "DC_motor.h"

extern uint8_t isMotorRightRUNNING  = TRUE;   // right motor = COM1A pin9 PB1
extern uint8_t isMotorLeftRUNNING   = TRUE;   // left motor = COM1B pin10 PB2

void init_DCmotors(void)
{
  DDRB  |= (1<<1) | (1<<2); // PORT B Pin 1 i 2 jako wyjścia ENA i ENB
  TCCR1A = (1<<WGM11) | (1<<WGM10);   // | (1<<COM1A1) | (1<<COM1B1);
  TCCR1B = (1<<CS12)  | (1<<WGM12);   // Tryb Fast PWM 10-bit TIMER1, przeskalowanie: clk/256 = 62.5kHz
  set_RightMotorSpeed (0);
  set_LeftMotorSpeed  (0);

  DDRD |= (1<<1) | (1<<4); // prawy silnik  PD1 = IN1 , PD4 = IN2
  DDRD |= (1<<5) | (1<<6); // lewy silnik   PD5 = IN3 , PD6 = IN4
}

void set_RightMotorSpeed(int motorRight)
{
  if(motorRight < 0)  // Jeśli joystick wychylony w dół - sterowanie do tyłu
  {
    motorRight *= -1;   // normalizacja wartości
    PORTD |= (1<<4);    // obroty do tyłu
    PORTD &= ~(1<<1);
  }
  else
  {
    PORTD |= (1<<1);    // obroty do przodu
    PORTD &= ~(1<<4);
  }
  
  if(motorRight > MOTOR_SPEED_MAX) motorRight = MOTOR_SPEED_MAX; // Ograniczenie
  if (motorRight <= 0 && isMotorRightRUNNING){
    TCCR1A  &= ~(1<<COM1A1);    // Wyłączenie sygnału PWM do pinu OC1A (PB1)
    PORTB   &= ~(1<<1);         // Wyłączenie stanu pina 1 na porcie B  na LOW
    isMotorRightRUNNING = FALSE;
  }
  else if (motorRight != 0 && !isMotorRightRUNNING){
    TCCR1A  |= (1<<COM1A1);     // Sygnał PWM do OC1A w trybie nieodwracającym (PB1)
    OCR1A   = motorRight;       // Ustawienie rejestru porównania (wartość do której doliczy)
    isMotorRightRUNNING = TRUE;
  }
  else if(isMotorRightRUNNING){
    OCR1A   = motorRight;       // Ustawienie rejestru porównania (wartość do której doliczy)
  }
}

void set_LeftMotorSpeed(int motorLeft)
{
  if(motorLeft < 0)   // Jeśli joystick wychylony w dół - sterowanie do tyłu
  {
    motorLeft *= -1;    // normalizacja wartości
    PORTD |= (1<<6);    // obroty do tyłu
    PORTD &= ~(1<<5);
  }
  else
  {
    PORTD |= (1<<5);    // obroty do przodu
    PORTD &= ~(1<<6);
  }
  
  if(motorLeft > MOTOR_SPEED_MAX) motorLeft = MOTOR_SPEED_MAX; // Ograniczenie
  if (motorLeft <= 0 && isMotorLeftRUNNING){ 
    TCCR1A  &= ~(1<<COM1B1);    // Wyłączenie sygnału PWM do pinu OC1B (PB2)
    PORTB   &= ~(1<<2);         // Ustawienie rejestru porównania (wartość do której doliczy)
    isMotorLeftRUNNING = FALSE;
  }
  else if (motorLeft != 0 && !isMotorLeftRUNNING){
    TCCR1A  |= (1<<COM1B1);     // Sygnał PWM do OC1A w trybie nieodwracającym (PB1)
    OCR1B   = motorLeft;        // Ustawienie rejestru porównania (wartość do której doliczy)
    isMotorLeftRUNNING = TRUE;
  }
  else if(isMotorLeftRUNNING){
    OCR1B   = motorLeft;        // Ustawienie rejestru porównania (wartość do której doliczy)
  }
}
