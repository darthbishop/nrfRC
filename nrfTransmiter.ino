//The setup function is called once at startup of the sketch

#include <SPI.h>
#include <LiquidCrystal.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "debug.h"
#include "payload.h"


//coment to disable debuging
#define DEBUG
// Set up nRF24L01 radio on SPI bus plus pins 9 & 10
RF24 radio(9,10);
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
payload p;
int battery;

// Radio pipe addresses for the 2 nodes to communicate.
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };

typedef enum {send,sent,recived,no_response} state_e;

state_e state=send;
unsigned long time;
char str[17];

void setup()
{

	Serial.begin(57600);
	debug(F("starting...\n"));

	pinMode(A2,INPUT_PULLUP);
	pinMode(A3,INPUT_PULLUP);

	lcd.begin(16, 2);
	lcd.print(F("starting..."));

	radio.begin();
	radio.setRetries(15,15);
	radio.setPayloadSize(17);
	radio.openWritingPipe(pipes[0]);
	radio.openReadingPipe(1,pipes[1]);
	radio.startListening();

#ifdef DEBUG
	radio.printDetails();
#endif
}


// The loop function is called in an endless loop
void loop()
{
	readInputs(p);
	battery=analogRead(A5);
	lcd.setCursor(0, 0);
	lcd.print("battery: "+String(battery));
	switch(state){
	case send:{
		radio.stopListening();
		bool ok = radio.write( &p, sizeof(p));
		time = millis();
		if (ok){
			state=sent;
			debug(F("ok..."));
		}
		else debug(F("failed.\n\r"));
		radio.startListening();
		break;
	}
	case sent:{
		if(radio.available()){
			radio.read(str,17);
			state=recived;
		}else{
			if(millis()-time>200)state=no_response;
		}
		break;
	}
	case recived:{
		lcd.setCursor(0, 1);
		lcd.print(str);
		state=send;
		break;
	}
	case no_response:{
		lcd.setCursor(0, 1);
		lcd.print(F("Not connected"));
		state=send;
		break;
	}

	}//end switch


}

