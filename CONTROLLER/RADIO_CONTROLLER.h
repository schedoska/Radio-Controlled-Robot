#ifndef RADIO_CONTROLLER_H
#define RADIO_CONTROLLER_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define RADIO_CONTROLLER_MAX_RECIVE_TRIES 1   //max ilosc prob odbioru infromacji
#define RADIO_CONTROLLER_LISTEN_MODE 0        //tryb nasłuchiwania
#define RADIO_CONTROLLER_SEND_MODE 1          //tryb wysyłania
#define RADIO_CONTROLLER_SEND_TO_REC_RATIO 8  //wspolcznink wysyłania do odbierania informacji

extern uint8_t radio_sendInRow;               //aktualna ilosc wysłań z rzędu
extern RF24 radio;
extern const uint8_t RADIO_CONTROLLER_READ_ADD[6];  //adres potoku do czytania
extern const uint8_t RADIO_CONTROLLER_WRIT_ADD[6];  //adres potoku do wpisu
extern uint8_t radio_current_mode;                  //tryb radia
extern uint8_t radio_controller_recive_tries;       //ilosc prób odbioru infromacji od robota

typedef struct
{
  int distance;             //dystans od przeszkody
  int level;                //poziom (roll)
  int temperature;          //natężenie światła
} RobotStateData;
typedef struct 
{
  int leftMotor;            //prędkośc lewego silnika
  int rightMotor;           //prędkośc prawego silnika
  uint8_t dataSendRequest;  //Rządenie przesułu stanu robota
} ControlData;

void radio_controller_init();
uint8_t radio_controller_sendControl(ControlData *control);
uint8_t radio_controller_tryReciveState(RobotStateData *state);
void radio_set_mode(uint8_t mode);
void radio_controller_handle_communication(ControlData *control, RobotStateData *state);
void radio_controller_modify_send_request(ControlData *control);

#endif //RADIO_CONTROLLER_H
