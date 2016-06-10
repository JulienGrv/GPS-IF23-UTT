//
// Created by Fox on 04/06/2015.
//

#include "DisplayManager.h"

LiquidCrystal DisplayManager::lcd = LiquidCrystal(LCDRS,LCDEN,LCDD4,
													LCDD5,LCDD6,LCDD7);
bool DisplayManager::initialized  = false;
int	 DisplayManager::display_mode = -1;
int	 DisplayManager::environment  = CLEAR;
int	 DisplayManager::samplerate	  = SRVL;
uint8_t	DisplayManager::current_menu = LOCKED;
unsigned long	 DisplayManager::iterations	  = 0;

void DisplayManager::init(){
	lcd.begin(8,2);
	delay(20);
	lcd.print(F("init"));
	initialized = true;
}
void DisplayManager::display(){
	lcd.clear();lcd.setCursor(0,0);
	if(display_mode==-2) {
		display_menu();
		return;
	}
	switch(display_mode) {
		case SETTINGS:
			display_settings();
			break;
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
		case STATISTICS:
			display_statistics();
			break;
		default:
			lcd.print(environment);lcd.print(GPSReader::satellites);
			lcd.print(' ');
			lcd.print(GPSReader::measures_done);
			lcd.setCursor(0,1);
			lcd.print(samplerate);
			if(current_menu==LOCKED){lcd.print(F("l "));}
			lcd.print(GPSReader::HDOP);
			return;
	}
}
void DisplayManager::display_menu(){
	if(current_menu==MAINMENU){lcd.print(F("mainmenu"));}
	if(current_menu==SETENV){lcd.print(F("environ"));}
	if(current_menu==SETSR){lcd.print(F("sample"));}
}
void DisplayManager::display_settings(){
	if(environment==CLEAR){
		lcd.print(F("clear"));
	}else if(environment==FOREST){
		lcd.print(F("forest"));
	}else if(environment==BUILDING){
		lcd.print(F("building"));
	}else if(environment==CAR){
		lcd.print(F("car"));
	}
	lcd.setCursor(0,1);
	lcd.print(samplerate);
}
void DisplayManager::display_datetime(){
	lcd.print(GPSReader::day);lcd.print('/');lcd.print(GPSReader::month);lcd.print('/');lcd.print(GPSReader::year%100);
	lcd.setCursor(0,1);
	lcd.print(GPSReader::hour);lcd.print(':');lcd.print(GPSReader::minutes);lcd.print(':');
	if(GPSReader::seconds<10){lcd.print('0');} lcd.print(GPSReader::seconds);
}
void DisplayManager::display_position(){
	lcd.print(GPSReader::longitude);
	lcd.setCursor(0,1);
	lcd.print(GPSReader::latitude);
}
void DisplayManager::display_altitude(){
	lcd.print(F("alt"));
	lcd.setCursor(0,1);
	lcd.print(GPSReader::altitude/100);
}
void DisplayManager::display_battery(){
	pinMode(BATTERY, INPUT);
	lcd.print(F("battery"));
	int analogValue = analogRead(BATTERY);
	lcd.setCursor(0,1);
	float voltage = (6.5*analogValue)/1023;
	lcd.print(voltage);
	voltage = 10*(voltage-4.5);
	lcd.print(' ');
	lcd.print(voltage);
}
void DisplayManager::display_statistics(){
	lcd.print(F("stats"));
}
