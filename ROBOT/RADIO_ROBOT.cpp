#include "RADIO_ROBOT.h"

RF24 radio(8, 7);
const uint8_t RADIO_ROBOT_READ_ADD[6] = "00001";        // adres odczytu
const uint8_t RADIO_ROBOT_WRIT_ADD[6] = "00002";        // adres wpisu
uint8_t radio_current_mode = RADIO_ROBOT_LISTEN_MODE;   // aktualny tryb
uint8_t radio_robot_send_tries = 0;                     // ilosć prób wysyłku danych

void radio_robot_init()
{
  radio.begin();
  radio.setPALevel(RF24_PA_MIN);  // ustawienie radia na niski poziom odległości i zużycia prądu
  
  radio.openWritingPipe(RADIO_ROBOT_WRIT_ADD);    // otwiera potok do wpisywania 
  radio.openReadingPipe(0, RADIO_ROBOT_READ_ADD); // otwiera potok do odczutu
  
  radio.startListening(); // rozpoczyna naslućhiwanie na przychodzące pakiety dnaych z kontrolera
  radio.flush_tx();       // czyści bufory FIFO transmisji 
  radio.flush_rx();       // czyści bufory FIFO odbioru 
}

uint8_t radio_robot_sendState(RobotStateData *state)
{
  return radio.write(state, sizeof(RobotStateData));  // wysyła strukturę state 
}

uint8_t radio_robot_tryReciveState(ControlData *control)
{
  if(radio.available()) // jeśli dostępne pakiety
    {
      radio.read(control, sizeof(ControlData));    
      return 1;
    }
  else return 0;
}

//  Ustawia tryb w jakim aktualnie działa robot nasłuchiwacza lub wysyłającego
//  RADIO_ROBOT_LISTEN_MODE -> słuchacz
//  RADIO_ROBOT_SEND_MODE -> wsyłający
void radio_set_mode(uint8_t mode)
{
  if(mode == RADIO_ROBOT_LISTEN_MODE)
  {
    radio.flush_rx();                               // czyści bufory FIFO odbioru  
    radio.startListening();                         // rozpoczyna nasłuchiwanie przychodzącyh danych
    radio.openReadingPipe(0, RADIO_ROBOT_READ_ADD); // otwiera potok do odczytu 
    radio_current_mode = RADIO_ROBOT_LISTEN_MODE;   // ustawia aktualny tryb działnia
  }
  else
  {
    radio.flush_tx();                               // czyści bufory FIFO transmiji
    radio.stopListening();                          // kończy nasłuchiwanie przychodzącyh danych
    radio.openWritingPipe(RADIO_ROBOT_WRIT_ADD);    // otwiera potok do wpisu
    radio_current_mode = RADIO_ROBOT_SEND_MODE;     // ustawia aktualny tryb działnia
  }
}

void radio_robot_handle_communication(RobotStateData *state, ControlData *control)
{
  if(radio_current_mode == RADIO_ROBOT_SEND_MODE)
  {
    if(radio_robot_sendState(state))
    {
      radio_set_mode(RADIO_ROBOT_LISTEN_MODE);              // powrót do trybu słuchacza (domyślny)
    }
    else
    {
      radio_robot_send_tries++;
    }
    if(radio_robot_send_tries > RADIO_ROBOT_MAX_SEND_TRIES) // Za duża ilość prób wysyłki bez powodzenia
    {
      radio_set_mode(RADIO_ROBOT_LISTEN_MODE);              // powrót do trybu słuchacza (domyślny)
    }
  }
  else
  {
    if(radio_robot_tryReciveState(control))
    {
      // BACKUP - USUN IF tylko te 2 linijki pod spodem
      if(control->dataSendRequest == 1)                     // jesli kontroler domaga się wysyłania pakietu zwrotnego
      {
        radio_set_mode(RADIO_ROBOT_SEND_MODE);
        radio_robot_send_tries = 0;
      }
    }
  }
}
