#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include "LiquidCrystal.h"

File file;
LiquidCrystal lcd(4,5,6,7,8,9);
char instruction[3]={'a','a','a'};
void setup() {
	Serial.begin(57600);
	lcd.begin(8,2);
	while(!SD.begin(10)){
		Serial.println(F("Could not initiate SD"));
	}
	delay(20);
	lcd.clear();
	lcd.setCursor(0,0);
	lcd.print(F("transfer"));
	lcd.setCursor(0,1);
	lcd.print(F("ready"));
}

void loop() {
	if(Serial.available()){
		instruction[0]=instruction[1];
		instruction[1]=instruction[2];
		instruction[2]=Serial.read();
		lcd.clear();
		lcd.setCursor(0,0);
		lcd.print(F("found"));
		lcd.setCursor(0,1);
		lcd.print(instruction[0]);
		lcd.print(instruction[1]);
		lcd.print(instruction[2]);
		if(instruction[0]=='r' && instruction[1]=='s' && instruction[2]=='t'){
			reset_files();
		}
		if(instruction[0]=='s' && instruction[1]=='a' && instruction[2]=='d'){
			send_all_data();
		}
	}
}

void send_all_data(){
	char* str[4]={"0","1","2","3"};
	for(int i = 0; i < 4; i++){
		file = SD.open(str[i], FILE_READ);
		if (file)
		{
			long filesize = file.size();
			long sent = 0, progress = -1, lastprog;
			Serial.print(i);Serial.print('|');
			lcd.clear();lcd.setCursor(0,0);lcd.print(F("file "));lcd.print(i);
			while(file.available()){
				lastprog=progress;
				Serial.write(file.read());
				sent++;
				lcd.setCursor(0,1);
				progress = 1000*sent/filesize;
				if(lastprog<progress){
					lcd.print(progress/10);lcd.print('.');
					lcd.print(progress%10);lcd.print('%');
				}
			}
			Serial.print('|');
		}
	}
	Serial.print("end");
	lcd.clear();
	lcd.setCursor(0,0);
	lcd.print(F("transfer"));
	lcd.setCursor(0,1);
	lcd.print(F("done"));
}

void reset_files(){
	char* str[4]={"0","1","2","3"};
	for(int i = 0; i < 4; i++){
		SD.remove(str[i]);
	}
}
