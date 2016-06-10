//
// Created by Fox on 04/06/2015.
//

#ifndef IF23MAIN_INO_DISPLAYMANAGER_H
#define IF23MAIN_INO_DISPLAYMANAGER_H

#include "define.h"
#include "GPSReader.h"
#include "LiquidCrystal.h"

class DisplayManager {
public:
	// FIELDS
	static bool initialized;
	static LiquidCrystal lcd;
	static int 	display_mode;
	static int 	environment;
	static int 	samplerate;
	static uint8_t current_menu;
	static unsigned long iterations;
	// METHODS
	static void init();
	static void display();
private:
	// FIELDS
	// METHODS
	static void display_settings();
	static void display_datetime();
	static void display_position();
	static void display_altitude();
	static void display_battery();
	static void display_statistics();
	static void display_menu();
};


#endif //IF23MAIN_INO_DISPLAYMANAGER_H
