#include "DC_motor.h"
#include "HC_SR04.h"
#include "RADIO_ROBOT.h"
#include "LIGHT_SENSOR.h"
#include "MPU_6050.h"

#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define SLEEP_TIME 150  

ControlData control = {0,0,0};  // Inicjalizacja struktury kontrolującej robota 
RobotStateData state = {0,0,0}; // Inicjalizacja struktury aktualnego stanu robota

void setup() {
  init_DCmotors();    // inicjalizacja PWM do sterowania silników 
  init_HCSR04();      // inicjalizacja obsługi modułu czujnika odleglosci
  radio_robot_init(); // inicjalizacja komunikacaji robota przez moduł radiowy nrf24l01
  init_lightSensor(); // inicjalizacja konwertera ADC do obsługi czujnika natęzenia swiatła

  init_MPU_6050();
}

void loop() {
  _delay_ms(SLEEP_TIME);
  
  startDistanceMeasure();                   // Rozpocznij pomiar odległosci
  state.distance = measuredDistance;        // Wpis wartości zmierzonej odległosci do struktury stanu

  read_lightSensor();                       // Wykonaj odczyt wartości 
  state.temperature = lightLevel;           // Wpis wartości natężenia światła do struktury stanu
  
  read_MPU_6050();
  state.level = measured_ACCEL_X;
  
  set_RightMotorSpeed (control.rightMotor); // ustawienie wartości prawego silnika ze struktury kontroli
  set_LeftMotorSpeed  (control.leftMotor);  // ustawienie wartości lewego silnika ze struktury kontroli

  radio_robot_handle_communication(&state, &control); // Wymiana danych z kontrolwerem*/
}
