#include <Arduino.h>
#include <SoftwareSerial.h>

// #include <SD.h>
// #include <SPI.h>
#include <TinyGPS++.h>
#include "define.h"
#include "DisplayManager.h"

// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(GPSRX, GPSTX);

/*
   From http://aprs.gids.nl/nmea/:

   $GPGSV

   GPS Satellites in view

   eg. $GPGSV,3,1,11,03,03,111,00,04,15,270,00,06,01,010,00,13,06,292,00*74
      $GPGSV,3,2,11,14,25,170,00,16,57,208,39,18,67,296,40,19,40,246,00*74
      $GPGSV,3,3,11,22,42,067,42,24,14,311,43,27,05,244,00,,,,*4D

   1    = Total number of messages of this type in this cycle
   2    = Message number
   3    = Total number of SVs in view
   4    = SV PRN number
   5    = Elevation in degrees, 90 maximum
   6    = Azimuth, degrees from true north, 000 to 359
   7    = SNR, 00-99 dB (null when not tracking)
   8-11 = Information about second SV, same as field 4-7
   12-15= Information about third SV, same as field 4-7
   16-19= Information about fourth SV, same as field 4-7
 */

TinyGPSCustom totalGPGSVMessages(gps, "GPGSV", 1); // $GPGSV sentence, 1st
                                                   // element
TinyGPSCustom messageNumber(gps, "GPGSV", 2); // $GPGSV sentence, 2nd element
TinyGPSCustom satsInView(gps, "GPGSV", 3); // $GPGSV sentence, 3rd element
TinyGPSCustom satNumber[4]; // to be initialized later
TinyGPSCustom elevation[4];
TinyGPSCustom azimuth[4];
TinyGPSCustom snr[4];

/*
   $GPGSA,A,3,04,05,,09,12,,,24,,,,,2.5,1.3,2.1*39

   Where:
   GSA      Satellite status
   A        Auto selection of 2D or 3D fix (M = manual)
   3        3D fix - values include: 1 = no fix
                                     2 = 2D fix
                                     3 = 3D fix
   04,05... PRNs of satellites used for fix (space for 12)
   2.5      PDOP (dilution of precision)
   1.3      Horizontal dilution of precision (HDOP)
   2.1      Vertical dilution of precision (VDOP)
 * 39      the checksum data, always begins with *
 */
TinyGPSCustom pdop(gps, "GPGSA", 15); // $GPGSA sentence, 15th element
TinyGPSCustom hdop(gps, "GPGSA", 16); // $GPGSA sentence, 16th element
TinyGPSCustom vdop(gps, "GPGSA", 17); // $GPGSA sentence, 17th element

// Optimization variables
const char virgule = ',';
int i              = 0;

// Structure satellites GSV
struct
{
  bool active;
  int  elevation;
  int  azimuth;
  int  snr;
} sats[MAX_SATELLITES];

// File file; // SD not working

// Display manager
int  measures_done      = 0;
bool swen               = false;
uint8_t sw              = 5;
uint8_t current_menu    = MAINMENU;
unsigned long last_menu = 0L;

void setup()
{
  Serial.begin(ARDUINOBAUD);
  ss.begin(GPSBAUD);

  DisplayManager::init();

  // Init SD
  // pinMode(SDSS, OUTPUT);
  // if (!SD.begin(SDSS)) {
  //   lcd.clear();
  //   lcd.setCursor(0, 0);
  //   lcd.print(F("SD error"));
  //
  //   while (true) {}
  // }

  // Initialize all the uninitialized TinyGPSCustom objects
  for (i = 0; i < 4; ++i)
  {
    satNumber[i].begin(gps, "GPGSV", 4 + 4 * i); // offsets 4, 8, 12, 16
    elevation[i].begin(gps, "GPGSV", 5 + 4 * i); // offsets 5, 9, 13, 17
    azimuth[i].begin(gps, "GPGSV", 6 + 4 * i); // offsets 6, 10, 14, 18
    snr[i].begin(gps, "GPGSV", 7 + 4 * i); // offsets 7, 11, 15, 19
  }
  i = 0;

  // Init buttons
  pinMode(SWEN, INPUT);
  pinMode(SWLO, INPUT);
  pinMode(SWHI, INPUT);
  delay(1000);
}

void loop()
{
  // Dispatch incoming characters
  while (ss.available() > 0)
  {
    gps.encode(ss.read());

    if (totalGPGSVMessages.isUpdated())
    {
      for (i = 0; i < 4; ++i)
      {
        int no = atoi(satNumber[i].value());

        // Serial.print(F("SatNumber is ")); Serial.println(no);
        if ((no >= 1) && (no <= MAX_SATELLITES))
        {
          sats[no - 1].elevation = atoi(elevation[i].value());
          sats[no - 1].azimuth   = atoi(azimuth[i].value());
          sats[no - 1].snr       = atoi(snr[i].value());
          sats[no - 1].active    = true;
        }
      }

      int totalMessages  = atoi(totalGPGSVMessages.value());
      int currentMessage = atoi(messageNumber.value());

      if (totalMessages == currentMessage)
      {
        measures_done++;

        // write_to_file(); // SD not working
        transmit();

        // Update LCD display
        menu_manager();
        DisplayManager::iterations   = measures_done;
        DisplayManager::satellites   = gps.satellites.value();
        DisplayManager::current_menu = current_menu;
        DisplayManager::display();

        for (i = 0; i < MAX_SATELLITES; ++i) {
          sats[i].active    = false;
          sats[i].elevation = 0;
          sats[i].azimuth   = 0;
          sats[i].snr       = 0;
        }
      }
    }
  }
}

// Transmit data via serial
void transmit() {
  Serial.print('[');
  Serial.print(gps.date.year());
  Serial.print(virgule);
  Serial.print(gps.date.month());
  Serial.print(virgule);
  Serial.print(gps.date.day());
  Serial.print(virgule);
  Serial.print(gps.time.hour());
  Serial.print(virgule);
  Serial.print(gps.time.minute());
  Serial.print(virgule);
  Serial.print(gps.time.second());
  Serial.print(virgule);
  Serial.print(gps.location.lat(), 6);
  Serial.print(virgule);
  Serial.print(gps.location.lng(), 6);
  Serial.print(virgule);
  Serial.print(gps.altitude.value());
  Serial.print(virgule);
  Serial.print(gps.satellites.value());
  Serial.print(virgule);
  Serial.print(gps.hdop.value());
  Serial.print(virgule);
  Serial.print(atof(pdop.value()), 3);
  Serial.print(virgule);
  Serial.print(atof(hdop.value()), 3);
  Serial.print(virgule);
  Serial.print(atof(vdop.value()), 3);
  Serial.print(virgule);
  Serial.print(satsInView.value());

  for (i = 0; i < MAX_SATELLITES; ++i)
    if (sats[i].active)
    {
      Serial.print(virgule);
      Serial.print(i + 1);
      Serial.print(virgule);
      Serial.print(sats[i].elevation);
      Serial.print(virgule);
      Serial.print(sats[i].azimuth);
      Serial.print(virgule);
      Serial.print(sats[i].snr);
    }
  Serial.print(']');
  Serial.print(';');
  Serial.println(F(""));
}

// Write to a file in SD
// void write_to_file() {
//   file = SD.open("save", FILE_WRITE);
//
//   if (file) {
//     file.print('[');
//     file.print(gps.date.year());
//     file.print(virgule);
//     file.print(gps.date.month());
//     file.print(virgule);
//     file.print(gps.date.day());
//     file.print(virgule);
//     file.print(gps.time.hour());
//     file.print(virgule);
//     file.print(gps.time.minute());
//     file.print(virgule);
//     file.print(gps.time.second());
//     file.print(virgule);
//     file.print(gps.location.lat());
//     file.print(virgule);
//     file.print(gps.location.lng());
//     file.print(virgule);
//     file.print(gps.altitude.value());
//     file.print(virgule);
//     file.print(gps.satellites.value());
//     file.print(virgule);
//     file.print(gps.hdop.value());
//
//     for (i = 0; i < MAX_SATELLITES; ++i)
//       if (sats[i].active)
//       {
//         file.print(virgule);
//         file.print(i + 1);
//         file.print(virgule);
//         file.print(sats[i].elevation);
//         file.print(virgule);
//         file.print(sats[i].azimuth);
//         file.print(virgule);
//         file.print(sats[i].snr);
//       }
//     file.print(']');
//     file.print(';');
//     file.println(F(""));
//   }
// }

// Read button and switch between screens (left button)
void menu_manager() {
  swen = digitalRead(SWEN);
  sw   = digitalRead(SWLO);

  if (swen && (last_menu < millis())) {
    if (current_menu == MAINMENU) {
      if (sw == HIGH) {
        DisplayManager::display_mode++;
        DisplayManager::display_mode = DisplayManager::display_mode <
                                       6 ? DisplayManager::display_mode : -2;
        last_menu = millis();
      }
    }
  }
}
