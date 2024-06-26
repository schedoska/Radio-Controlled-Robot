#ifndef LCD_H
#define LCD_H

#include <avr/io.h>
#include "RADIO_CONTROLLER.h"

#define RS        1              // RS - PC1
#define RW        2              // RW - PC2
#define EN        0              // EN - PC0
#define Data_PORT PORTD                                                  
#define CTRL_PORT PORTC

void lcd_init(void);              //incjalizacja wyswietlacza
void lcd_cmd (char cmd);          //wpisz polecenie do LCD
void lcd_data(char data);         //wpisz znak char do LCd
void lcd_clear(void);             //wyczyśc ekran
void lcd_goto2line(void);         //idz to 2 lini LCd
void lcd_loadText(char *data);    //wpisz ciag znakowd do LCD

// ------------------------- CONTROLLER INTERFACE ----------------

#define LCD_INTERFACE_BUTTON_NEEDED_LENGHTH 1  //filtr przycisku jesli 1 to nieuzywany
#define LCD_INTERFACE_MAX_PAGES 3

#define DISTANCE_PAGE 0
#define LEVEL_PAGE 1
#define TEMP_PAGE 2

extern uint8_t interfaceCurrentPage;        //akualna strona wyś.
extern uint8_t interfacePreviousPage;       //porzednia strona
extern uint8_t currentButtonClickedCycles;  //ilosc cykli przycisku (filtracja)
extern uint8_t isStartEvent;                //flaga infromująca czy to inicjalizacja

void lcd_controller_interface_init();
void lcd_controller_update(RobotStateData *data);
void lcd_controller_updateDistancePage(RobotStateData *data);   //page 0
void lcd_controller_updateLevelPage(RobotStateData *data);      //page 1
void lcd_controller_updateLuxPage(RobotStateData *data);        //page 2

void lcd_print_currentPage();

#endif
