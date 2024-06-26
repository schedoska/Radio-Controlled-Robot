#ifndef JOYSTICK_H
#define JOYSTICK_H
#include <avr/io.h>

#define CENTERE_OFFSET 500  //wartośc okreslająca srodek ukladu wspolrzednych joysticka
#define FINAL_SCALER 1.4    //parametr skalowania
#define IDLE_TOLERANCE 100  //obszar zerowania silnikow

struct JoyStickPos{
  int Steer;  //Wychylenie osi X
  int Speed;  //Wychylenie osi Y
};
extern JoyStickPos _joystickPos;

void init_Joystick();   // inicjalizacja joysticka  
void read_joystick();   // odczyt wartości wychyleń joysticka
void getMotorValues(int *rightMotor, int *leftMotor); // konwersja wychyleń joysticka na sterowania silnikami
 
#endif //JOYSTICK_H
