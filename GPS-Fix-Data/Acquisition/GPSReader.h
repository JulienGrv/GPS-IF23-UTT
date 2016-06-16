#ifndef GPSREADER_H
#define GPSREADER_H
#include "TinyGPS.h"
#include "define.h"
#include <SoftwareSerial.h>

class GPSReader
{
public:
	// FIELDS
	static int		year;
	static byte 	month;
	static byte 	day;
	static byte 	hour;
	static byte 	minutes;
	static byte 	seconds;
	static long		latitude;
	static long		longitude;
	static long		altitude;
	static byte		satellites;
	static unsigned long HDOP;
	static unsigned long speed;
	static unsigned long course;
	static unsigned long measures_done;
	static unsigned long characters;
	static bool		new_point;
	// METHODS
	static void init();
	static void measure();
	inline static long mean_HDOP(){ long avr = sumHDOP / sample_number; sumHDOP = 0; sample_number = 0; return avr; }
private:
	static TinyGPS tinygps;
	static SoftwareSerial nss;
	static int  sample_number;
	static unsigned long sumHDOP;
};

#endif
