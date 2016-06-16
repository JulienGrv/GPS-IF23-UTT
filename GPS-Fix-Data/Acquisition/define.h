#ifndef DEFINE_H
#define DEFINE_H

//	PIN NUMBERS
// GPS
#define		GPSTX			2
#define		GPSRX			3
// LCD
#define		LCDRS			4
#define		LCDEN			5
#define		LCDD4			6
#define		LCDD5			7
#define		LCDD6			8
#define		LCDD7			9
// SD
#define		SDSS			10
#define		SDMOSI			11
#define		SDMISO			12
#define		SDCK			13
// BATTERY
#define		BATTERY			14
// BUTTONS
#define		SWEN			17
#define		SWLO			16
#define		SWHI			15


// 	ENUMERATION MENUS 
#define     MAINMENU		1
#define     SETENV         	2
#define     SETSR			3
#define		LOCKED			0

// ENUMERATION ENVIRONMENT 
#define     CLEAR           0
#define     FOREST          1
#define     BUILDING        2
#define     CAR             3


// ENUMERATION DISPLAY MODE 
#define     SETTINGS        0
#define     DATETIME        1
#define     POSITION        2
#define		ALTITUDE		3
#define     BATTERYM        4
#define     STATISTICS      5


// SAMPLERATES (MS)
#define     SRVL        	600000L
#define     SRL         	120000L
#define     SRH         	30000L
#define     SRVH        	2000L


#endif