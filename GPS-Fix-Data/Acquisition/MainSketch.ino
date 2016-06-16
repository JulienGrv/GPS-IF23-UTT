#include <EEPROM.h>
#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <SoftwareSerial.h>
#include "DisplayManager.h"
#include "GPSReader.h"
#include "define.h"
#include "LiquidCrystal.h"


File file;
long samplerate  = SRVL;
uint8_t  environment = CLEAR, sr = 0;
const char virgule = ',';
bool swen = false; bool lswen = false; uint8_t sw = 5;
uint8_t current_menu = LOCKED;
unsigned long iteration = 0L;
unsigned long last_save = 0L;
unsigned long last_menu = 0L;
unsigned long last_swms = 0L;
void setup() {
	if(EEPROM.read(0) == 125){
		environment = EEPROM.read(1);
		sr = EEPROM.read(2);
	}
	else{
		EEPROM.write(0, 125);
		EEPROM.write(1, CLEAR);
		EEPROM.write(2, 0);
	}
	if(sr == 0){samplerate = SRVL;}
	if(sr == 1){samplerate = SRL;}
	if(sr == 2){samplerate = SRH;}
	if(sr == 3){samplerate = SRVH;}
	DisplayManager::init();
	GPSReader::init();
	Serial.begin(9600);
	if(!SD.begin(SDSS)){
		while(1){}
	}
	pinMode(SWEN,INPUT);
	pinMode(SWLO,INPUT);
	pinMode(SWHI,INPUT);
	delay(1000);
}

void loop() {
	if(iteration%100==0){
		menu_manager();
		DisplayManager::environment = environment;
		DisplayManager::samplerate = sr;
		DisplayManager::iterations = iteration;
		DisplayManager::current_menu = current_menu;
		DisplayManager::display();
	}
	GPSReader::measure();
	unsigned long t = millis() / samplerate;
	if(last_save < t && GPSReader::new_point){
		write_to_file();
		last_save = t;
		GPSReader::new_point = 0;
	}
	iteration++;
}

void menu_manager() {
	if(last_swms<millis()-10){
		lswen = swen;
		swen = digitalRead(SWEN);
		sw = digitalRead(SWLO) + (digitalRead(SWHI)<<1);
		last_swms = millis();
	}
	if(current_menu == LOCKED) {
		if(!lswen && swen && sw == 2 && last_menu < millis()) {
			last_menu = millis();
		}
		if(lswen && last_menu < millis() - 1000 && swen && sw == 2){
			current_menu = MAINMENU;
			DisplayManager::display_mode = -2;
			last_menu = millis();
		}
	} else if(last_menu < millis() - 15000){ last_menu = millis(); current_menu = LOCKED; DisplayManager::display_mode = -1;}
	if(!lswen && swen && last_menu < millis()){
		if(current_menu == MAINMENU){
			if(sw == 0){ DisplayManager::display_mode = -2; current_menu = SETENV; last_menu = millis(); }
			if(sw == 1){ DisplayManager::display_mode = -2; current_menu = SETSR; last_menu = millis(); }
			if(sw == 2 && last_menu < millis() - 1000){ last_menu = millis(); current_menu = LOCKED; DisplayManager::display_mode = -1; }
			if(sw == 3){ 
				DisplayManager::display_mode++;
				DisplayManager::display_mode = DisplayManager::display_mode < 6 ? DisplayManager::display_mode : -2;
				last_menu = millis(); 
			}
		}
		else if(current_menu == SETENV){
			environment = sw;
			EEPROM.write(1,environment);
			current_menu = MAINMENU;
			last_menu = millis();
		}
		else if(current_menu == SETSR){
			sr = sw;
			if(sr == 0){samplerate = SRVL;}
			if(sr == 1){samplerate = SRL;}
			if(sr == 2){samplerate = SRH;}
			if(sr == 3){samplerate = SRVH;}
			EEPROM.write(2,sr);
			current_menu = MAINMENU;
			last_menu = millis();
		}
	}
}

void write_to_file() {
	if(2015!=GPSReader::year){return;}
	if(environment==CLEAR){
		file = SD.open("0", FILE_WRITE);
	} else if(environment==FOREST){
		file = SD.open("1", FILE_WRITE);
	} else if(environment==BUILDING){
		file = SD.open("2", FILE_WRITE);
	} else if(environment==CAR){
		file = SD.open("3", FILE_WRITE);
	}
	if(!file){
		DisplayManager::lcd.print("!file");
		delay(2000);
		return;
	}
	GPSReader::measures_done++;
	file.print('[');
	file.print(GPSReader::year);
	file.print(virgule);
	file.print(GPSReader::month);
	file.print(virgule);
	file.print(GPSReader::day);
	file.print(virgule);
	file.print(GPSReader::hour);
	file.print(virgule);
	file.print(GPSReader::minutes);
	file.print(virgule);
	file.print(GPSReader::seconds);
	file.print(virgule);
	file.print(GPSReader::latitude);
	file.print(virgule);
	file.print(GPSReader::longitude);
	file.print(virgule);
	file.print(GPSReader::altitude);
	file.print(virgule);
	file.print(GPSReader::satellites);
	file.print(virgule);
	file.print(GPSReader::HDOP);
	file.print(virgule);
	file.print(GPSReader::mean_HDOP());
	file.print(virgule);
	file.print(GPSReader::speed);
	file.print(virgule);
	file.print(GPSReader::course);
	file.print(']');
	file.print(';');
	file.close();
}
