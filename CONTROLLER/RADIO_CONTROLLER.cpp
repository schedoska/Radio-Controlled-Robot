#include "RADIO_CONTROLLER.h"

RF24 radio(9, 8);
const uint8_t RADIO_CONTROLLER_READ_ADD[6] = "00002";     // adres odczytu
const uint8_t RADIO_CONTROLLER_WRIT_ADD[6] = "00001";     // adres wpisu

uint8_t radio_current_mode = RADIO_CONTROLLER_SEND_MODE;  // aktualny tryb nadawania/słuchania kontrolera
uint8_t radio_controller_recive_tries = 0;                // ilosc prób odbioru danych od robota
uint8_t radio_sendInRow = 0;                              // ilosc wysłanych pakietów z rzeędu (rządanie zwrotu)

void radio_controller_init()
{
  radio.begin();
  radio.setPALevel(RF24_PA_MIN);                          // ustawienie radia na niski poziom odległości i zużycia prądu

  radio.openReadingPipe(0, RADIO_CONTROLLER_READ_ADD);    // otwiera potok do oczytu 
  radio.openWritingPipe(RADIO_CONTROLLER_WRIT_ADD);       // otwiera potok do wpisywania 
  
  radio.stopListening();                                  // uruvhomienie trybu nasłuvhiwania nadhcodżacyh pakietów
  radio.flush_tx();                                       // czyści bufory FIFO transmisji 
  radio.flush_rx();                                       // czyści bufory FIFO odbioru 
}

uint8_t radio_controller_sendControl(ControlData *control)
{
  uint8_t send_status = radio.write(control, sizeof(ControlData));  // Wysyła strukturę kontroli
  if(send_status == 1)
  {
    return 1;
  }
  else return 0;
}

uint8_t radio_controller_tryReciveState(RobotStateData *state)
{
  if(radio.available())
    {
      radio.read(state, sizeof(RobotStateData));    
      return 1;
    }
  else return 0;
}

// Ustawia tryb w jakim aktualnie działa robot nasłuchiwacza lub wysyłającego
//  RADIO_CONTROLLER_LISTEN_MODE -> słuchacz
//  RADIO_CONTROLLER_SEND_MODE -> wsyłający
void radio_set_mode(uint8_t mode)
{
  if(mode == RADIO_CONTROLLER_LISTEN_MODE)
  {
    radio.flush_rx();                                     // czyści bufory FIFO odbioru 
    radio.startListening();                               // rozpoczyna nasłuchiwanie przychodzącyh danych
    radio.openReadingPipe(0, RADIO_CONTROLLER_READ_ADD);  // otwiera potok do odczytu 
    radio_current_mode = RADIO_CONTROLLER_LISTEN_MODE;    // ustawia aktualny tryb działnia
  }
  else
  {
    radio.flush_tx();                                     // czyści bufory FIFO transmisji
    radio.stopListening();                                // przechodzi w tryb wysyłu danych
    radio.openWritingPipe(RADIO_CONTROLLER_WRIT_ADD);     // otwiera potok do wpisu
    radio_current_mode = RADIO_CONTROLLER_SEND_MODE;      // ustawia aktualny tryb działnia
  }
}

void radio_controller_handle_communication(ControlData *control, RobotStateData *state)
{ 
  if(radio_current_mode)
  {
    if(radio_controller_sendControl(control))
    {
      if(control->dataSendRequest == 1)               // jeśli rząda od robota odesłania struktury stanu
      {
        radio_controller_recive_tries = 0;            // Zeruje ilość prób odbioru pakietów od robota
        radio_set_mode(RADIO_CONTROLLER_LISTEN_MODE); // przechodzi w tryb słuchacza (domyślny)
      }
    }
  }
  else
  {
    if(radio_controller_tryReciveState(state))
    {
      radio_set_mode(RADIO_CONTROLLER_SEND_MODE); // Przejście w tryb nadawcy 
    }
    else
    {
      radio_controller_recive_tries++;
    }
    if(radio_controller_recive_tries > RADIO_CONTROLLER_MAX_RECIVE_TRIES) // Za duża ilość prób odbioru danych od robota bez powodzenia
    {
      radio_set_mode(RADIO_CONTROLLER_SEND_MODE); // Przejście w tryb nadawcy 
    }
  }
}

//Funkcja edutuje strukturę ControData w zależności od tego czy aktualnie
//kontroler domaga się danych zwrotnych od robota wedle ustawnienia makra
//RADIO_CONTROLLER_SEND_TO_REC_RATIO - ustalającego co ile cykli kontroler
//rząda infromacji zwrotenj
void radio_controller_modify_send_request(ControlData *control)
{
  if(radio_sendInRow >= RADIO_CONTROLLER_SEND_TO_REC_RATIO) // Jeśli ilość cykli przekroczy ustaloną wartość 
  {
    radio_sendInRow = 0;
    control->dataSendRequest = 1; // Modyfikuje strukture kontroli tak by robot zwrócił informacje o swoim stanie
  }
  else
  {
    control->dataSendRequest = 0; // Modyfikuje strukture kontroli tak by robot nie zwracał informacji
    radio_sendInRow++;
  }
}
