#include "LCD.h"
#include "RADIO_CONTROLLER.h"
#include "Joystick.h"

#define SLEEP_TIME 150 

ControlData control = {0,0,1};    // Inicjalizacja struktury kontroli
RobotStateData state = {0,0,0};   // Inicjalizacja struktury stanu robota

void setup() {  
  lcd_init();                       // Inicjalizacja ekranu lcd
  lcd_clear();                      // Wyczyszczenie zawartości ekranu
  radio_controller_init();          // Inicjalizacja komunikacji radiowej kontrolera z robotem
  init_Joystick();                  // INicjalizacja modułu joysticka
  lcd_controller_interface_init();  // INicjalizacja interfejsu ekranu lcd
}

void loop() {
  radio_sendInRow++;                // aktualizacja wartości śledzącej cykle (do komunikacji radiowej)

  radio_controller_modify_send_request(&control);           // modyfikacja struktury kontroli (decyzja czy wysłać zapytanie o stan robota)
  radio_controller_handle_communication(&control, &state);  // obsługa komunikacji robota z kontrolerem

  lcd_controller_update(&state);    // aktualzacja interfejsu kontrolera

  read_joystick();                  // odczyt pozycji joysticka
  getMotorValues(&control.rightMotor, &control.leftMotor);  // translacja i przypisanie wartości obrotów silników

  _delay_ms(SLEEP_TIME);
}
