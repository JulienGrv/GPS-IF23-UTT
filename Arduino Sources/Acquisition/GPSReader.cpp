#include "GPSReader.h"

//	FIELD INIT
// Public Fields
int		GPSReader::year			 = 0;
byte	GPSReader::month		 = 0;
byte	GPSReader::day			 = 0;
byte	GPSReader::hour			 = 0;
byte	GPSReader::minutes		 = 0;
byte	GPSReader::seconds		 = 0;
long	GPSReader::latitude		 = 0;
long	GPSReader::longitude	 = 0;
long	GPSReader::altitude		 = 0;
byte	GPSReader::satellites	 = 0;
unsigned long	GPSReader::HDOP	 = 0;
unsigned long	GPSReader::speed = 0;
unsigned long	GPSReader::course= 0;
unsigned long	GPSReader::measures_done = 0;
unsigned long	GPSReader::characters = 0;
bool	GPSReader::new_point	 = 0;
// Private Fields
int		GPSReader::sample_number = 0;
unsigned long GPSReader::sumHDOP = 0;
TinyGPS GPSReader::tinygps;
SoftwareSerial GPSReader::nss(GPSRX,GPSTX);

//	METHODS
// Public Methods
void GPSReader::init(){
	nss = SoftwareSerial(GPSRX,GPSTX);
	nss.begin(4800);
}

void GPSReader::measure(){
	bool full_sentence = false;
	while(nss.available()){
		char c = nss.read();
		characters++;
		Serial.print(c);
		full_sentence = tinygps.encode(c);
		if(full_sentence){
			Serial.println(F("FULL"));
			break;
		}
	}
	if(!full_sentence){return;}
	Serial.println(F("Interpreting"));
	tinygps.get_position(&latitude,&longitude);
	tinygps.crack_datetime(&year,&month,&day,&hour,&minutes,&seconds);
	altitude = tinygps.altitude();
	satellites = (byte)tinygps.satellites();
	HDOP = tinygps.hdop();
	sumHDOP += HDOP;
	speed = tinygps.speed();
	course = tinygps.course();
	sample_number++;
	new_point = 1;
}


// Private Methods
