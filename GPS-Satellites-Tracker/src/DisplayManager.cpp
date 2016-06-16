#include "DisplayManager.h"

LiquidCrystal DisplayManager::lcd = LiquidCrystal(LCDRS,
                                                  LCDEN,
                                                  LCDD4,
                                                  LCDD5,
                                                  LCDD6,
                                                  LCDD7);
int DisplayManager::display_mode         = -1;
uint8_t DisplayManager::current_menu     = MAINMENU;
unsigned long DisplayManager::iterations = 0;
int DisplayManager::year                 = 0;
byte   DisplayManager::month             = 0;
byte   DisplayManager::day               = 0;
byte   DisplayManager::hour              = 0;
byte   DisplayManager::minutes           = 0;
byte   DisplayManager::seconds           = 0;
double DisplayManager::latitude          = 0;
double DisplayManager::longitude         = 0;
long   DisplayManager::altitude          = 0;
byte   DisplayManager::satellites        = 0;

// Init LCD display
void DisplayManager::init() {
  lcd.begin(8, 2);
  delay(20);
  lcd.print(F("init"));
}

// Disp error
void DisplayManager::error() {
  delay(20);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("SD error"));
}

// Disp a specific screen
void DisplayManager::display() {
  lcd.clear(); lcd.setCursor(0, 0);

  if (display_mode == -2) {
    display_menu();
    return;
  }

  switch (display_mode) {
  case DATETIME:
    display_datetime();
    break;

  case POSITION:
    display_position();
    break;

  case ALTITUDE:
    display_altitude();
    break;

  case BATTERYM:
    display_battery();
    break;

  default:
    lcd.print(satellites);
    lcd.setCursor(0, 1);
    lcd.print(iterations);
    return;
  }
}

// Disp main mun
void DisplayManager::display_menu() {
  if (current_menu == MAINMENU) lcd.print(F("mainmenu"));
}

// Disp date time
void DisplayManager::display_datetime() {
  lcd.print(day); lcd.print('/'); lcd.print(month);
  lcd.print('/'); lcd.print(year % 100);
  lcd.setCursor(0, 1);
  lcd.print(hour); lcd.print(':'); lcd.print(minutes);
  lcd.print(':');

  if (seconds < 10) lcd.print('0');  lcd.print(seconds);
}

// Disp position
void DisplayManager::display_position() {
  lcd.print(longitude);
  lcd.setCursor(0, 1);
  lcd.print(latitude);
}

// Disp altitude
void DisplayManager::display_altitude() {
  lcd.print(F("alt"));
  lcd.setCursor(0, 1);
  lcd.print(altitude / 100);
}

// Disp battery
void DisplayManager::display_battery() {
  pinMode(BATTERY, INPUT);
  lcd.print(F("battery"));
  int analogValue = analogRead(BATTERY);
  lcd.setCursor(0, 1);
  float voltage = (6.5 * analogValue) / 1023;
  lcd.print(voltage);
  voltage = 10 * (voltage - 4.5);
  lcd.print(' ');
  lcd.print(voltage);
}
