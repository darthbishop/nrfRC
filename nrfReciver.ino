#include <SPI.h>

#include "nRF24L01.h"
#include "RF24.h"
#include "debug.h"
#include "payload.h"
//coment to disable debuging
#define DEBUG
// Set up nRF24L01 radio on SPI bus plus pins 9 & 10
RF24 radio(9,10);

payload p;
int battery;

// Radio pipe addresses for the 2 nodes to communicate.
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };

typedef enum {send,sent,recived,no_response} state_e;

state_e state=send;
unsigned long time;
String str;

void setup()
{
	str.reserve(17);
	Serial.begin(57600);
	Serial.print(F("starting...\n"));

	radio.begin();
	radio.setRetries(15,15);
	radio.setPayloadSize(17);
	radio.openWritingPipe(pipes[1]);
	radio.openReadingPipe(1,pipes[0]);
	radio.startListening();

//#ifdef DEBUG
	radio.printDetails();
//#endif
}


void loop()
{
	if ( radio.available() ){
		radio.read(&p,sizeof (p));
	
	str=String(p.chanel1)+" "+String(p.chanel2)+" "+String(p.button1)+" "+String(p.button2);
	radio.stopListening();

	radio.write( str.c_str(), 17);
	Serial.print(F("Sent response.\n\r"));

	// Now, resume listening so we catch the next packets.
	radio.startListening();
	}
}
