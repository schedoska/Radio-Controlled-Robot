#include "LCD.h"

#include <avr/io.h>
#include <avr/delay.h>

uint8_t interfaceCurrentPage = DISTANCE_PAGE; 
uint8_t currentButtonClickedCycles = 0;
uint8_t interfacePreviousPage = -1;
uint8_t isStartEvent = 1;

void lcd_init(void)
{
  DDRD |= 0xF0;       // Konfiguracja wyjsc do portu data
  DDRC |= 0x03;       // Konfiguracja wyjsc do portu kontroli -> RS RW EN
  _delay_ms(20);
  CTRL_PORT &= ~(1<<RS);
  lcd_cmd(0x02);       // inicjalizacja w trybie 4-bit
  lcd_cmd(0x28);       // tryb 4-bit, w linie, 5:7 rozmiar znaku
  lcd_cmd(0x0C);       // bez widocznego kursora
  lcd_cmd(0x06);       // przesuwanie kursora po dodaniu znaku -> auto
  lcd_cmd(0x01);       // Czysci ekran
  lcd_cmd(0x80);       // Poczatke textu (1 linijka, 1 znak)
  _delay_ms(5);
}

void lcd_cmd (char cmd)
{
  CTRL_PORT &= ~(1<<RS);                                  // tryb przesyłu polecenia
  Data_PORT = (Data_PORT & ~(0xF0)) | (cmd & 0xF0);       // Wstawienie na PORTD górnej części bajtu polecenia 0b11110000
  CTRL_PORT |= (1<<EN);                                   // wpis do rejestrów górnych 4 bitów polecenia (impuls EN)
  CTRL_PORT &= ~(1<<EN);                                  
  Data_PORT = (Data_PORT & ~(0xF0)) | ((cmd<<4) & 0xF0);  // Wstawienie na PORTD dolnej części bajtu polecenia 0b00001111 
  CTRL_PORT |= (1<<EN);                                   // wpis do rejestrów dolnych 4 bitów polecenia (impuls EN)
  CTRL_PORT &= ~(1<<EN);
  _delay_us(50);
}

void lcd_data(char data)
{
  CTRL_PORT |= (1<<RS);                                   // tryb przesyłu polecenia
  Data_PORT = (Data_PORT & ~(0xF0)) | (data & 0xF0);      // Wstawienie na PORTD górnej części bajtu data 0b11110000
  CTRL_PORT |= (1<<EN);                                   // wpis do rejestrów górnych 4 bitów bajtu data (impuls EN)
  CTRL_PORT &= ~(1<<EN);
  Data_PORT = (Data_PORT & ~(0xF0)) | ((data<<4) & 0xF0); // Wstawienie na PORTD dolnej części bajtu data 0b00001111 
  CTRL_PORT |= (1<<EN);                                   // wpis do rejestrów dolnych 4 bitów data (impuls EN)
  CTRL_PORT &= ~(1<<EN);  
  _delay_us(50);
}

void lcd_clear(void)
{
  lcd_cmd(0x01);                   // 0x01 -> polecenie czyszczace zawartość rejestrów lcd
  _delay_ms(2);
}

void lcd_goto2line(void)
{
  lcd_cmd(0xc0);                   // 0xc0 -> polecenie przejscia do drugiej linii pierszy znak
}

void lcd_loadText(char *data)
{
  while(*data)
  {
    lcd_data(*data);
    data++;
  }
}
// --------------------------------------------------------- INTERFACE ------------------------------------------------------
void lcd_controller_interface_init()
{
  DDRD &= ~(1<<2);   //inicjalizacja przycisku zmiany strony
  PORTD |= (1<<2);
}

void lcd_controller_update(RobotStateData *data)
{
  interfacePreviousPage = interfaceCurrentPage;
  if((PIND & (1<<2)) == 0)
  {
    currentButtonClickedCycles++;
    if(currentButtonClickedCycles >= LCD_INTERFACE_BUTTON_NEEDED_LENGHTH)
    {
      currentButtonClickedCycles = 0;
      interfaceCurrentPage = (++interfaceCurrentPage) % LCD_INTERFACE_MAX_PAGES;
    }
  }
  else
  {
    currentButtonClickedCycles = 0;
  }

  switch(interfaceCurrentPage)
  {
    case DISTANCE_PAGE:
      lcd_controller_updateDistancePage(data);
      break;
    case LEVEL_PAGE:
      lcd_controller_updateLevelPage(data);
      break;
    case TEMP_PAGE:
      lcd_controller_updateLuxPage(data);
      break;
    default:
      break;
  }
}

void lcd_controller_updateDistancePage(RobotStateData *data)
{
  if(interfacePreviousPage != DISTANCE_PAGE || isStartEvent)
  {
    isStartEvent = 0;           //wyzerownie wyfdarzenia startowego
    lcd_clear();                
    lcd_loadText("Distance: ");
    lcd_cmd (0x8D);
    lcd_loadText("cm");
    lcd_print_currentPage();
  }

  float dist_tansformed = data->distance * 0.1f + 0.75;
  lcd_cmd (0x8A);
  char setki = (dist_tansformed / 100) + 48;
  char dzies = ((dist_tansformed - (setki - 48) * 100) / 10) + 48;
  char jedynki = dist_tansformed - (setki - 48) * 100 - (dzies - 48) * 10 + 48;

  if(setki - 48 > 0)
    lcd_data(setki);
  else lcd_data(' ');
  if(dzies - 48 > 0 || setki - 48 > 0)
    lcd_data(dzies);
  else lcd_data(' ');
  lcd_data(jedynki);
}

void lcd_controller_updateLevelPage(RobotStateData *data)
{
  if(interfacePreviousPage != LEVEL_PAGE)
  {
    lcd_clear();
    lcd_loadText("Level: ");
    lcd_cmd (0x8C);
    lcd_data(0xDF);
    lcd_data(223);
    lcd_print_currentPage();
  }

  lcd_cmd(0x89);
  uint8_t isNegative = 0;
  int level_copy = data->level;
  if(level_copy < 0)
  {
    level_copy *=-1; 
    isNegative = 1;
    lcd_data(45);
  }
  else
  {
    lcd_data(' ');
  }
  
  lcd_cmd (0x8A);
  char setki = (level_copy / 100) + 48;
  char dzies = ((level_copy - (setki - 48) * 100) / 10) + 48;
  char jedynki = level_copy - (setki - 48) * 100 - (dzies - 48) * 10 + 48;

  if(setki - 48 > 0)
    lcd_data(setki);
  else lcd_data(' ');
  if(dzies - 48 > 0 || setki - 48 > 0)
    lcd_data(dzies);
  else lcd_data(' ');
  lcd_data(jedynki);
}

void lcd_controller_updateLuxPage(RobotStateData *data)
{
  if(interfacePreviousPage != TEMP_PAGE)
  {
    lcd_clear();
    lcd_loadText("Light: ");
    lcd_cmd (0x8D);
    lcd_loadText("u");
    lcd_print_currentPage();
  }

  lcd_cmd (0x8A);
  char setki = (data->temperature / 100) + 48;
  char dzies = ((data->temperature - (setki - 48) * 100) / 10) + 48;
  char jedynki = data->temperature - (setki - 48) * 100 - (dzies - 48) * 10 + 48;

  if(setki - 48 > 0)
    lcd_data(setki);
  else lcd_data(' ');
  if(dzies - 48 > 0 || setki - 48 > 0)
    lcd_data(dzies);
  else lcd_data(' ');
  lcd_data(jedynki);
}
// wyswietla aktualna strona
void lcd_print_currentPage()
{
  lcd_cmd (0xC0);
  lcd_data('[');
  lcd_data(interfaceCurrentPage + 49);
  lcd_data('/');
  lcd_data(LCD_INTERFACE_MAX_PAGES + 48);
  lcd_data(']');
}
