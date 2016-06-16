#ifndef DISPLAYMANAGER_H
#define DISPLAYMANAGER_H

#include <LiquidCrystal.h>
#include <Arduino.h>
#include "define.h"

class DisplayManager {
public:

  // FIELDS
  static LiquidCrystal lcd;
  static int display_mode;
  static uint8_t current_menu;
  static unsigned long iterations;
  static int year;
  static byte   month;
  static byte   day;
  static byte   hour;
  static byte   minutes;
  static byte   seconds;
  static double latitude;
  static double longitude;
  static long   altitude;
  static byte   satellites;

  // METHODS
  static void init();
  static void display();
  static void error();

private:

  // FIELDS


  // METHODS
  static void display_datetime();
  static void display_position();
  static void display_altitude();
  static void display_battery();
  static void display_menu();
};

#endif // DISPLAYMANAGER_H
