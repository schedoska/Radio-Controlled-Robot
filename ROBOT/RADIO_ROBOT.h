#ifndef RADIO_ROBOT_H
#define RADIO_ROBOT_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define RADIO_ROBOT_MAX_SEND_TRIES 4
#define RADIO_ROBOT_LISTEN_MODE 0
#define RADIO_ROBOT_SEND_MODE 1

extern RF24 radio;
extern const uint8_t RADIO_ROBOT_READ_ADD[6];
extern const uint8_t RADIO_ROBOT_WRIT_ADD[6]; 
extern uint8_t radio_current_mode;
extern uint8_t radio_robot_send_tries;

typedef struct
{
  int distance;
  int level;
  int temperature;
} RobotStateData;
typedef struct 
{
  int leftMotor;
  int rightMotor;
  uint8_t dataSendRequest;
} ControlData;

void radio_robot_init();
uint8_t radio_robot_sendState(RobotStateData *state);
uint8_t radio_robot_tryReciveState(ControlData *control);
void radio_set_mode(uint8_t mode);
void radio_robot_handle_communication(RobotStateData *state, ControlData *control);

#endif //RADIO_ROBOT_H
