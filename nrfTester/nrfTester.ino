//The setup function is called once at startup of the sketch

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "payload.h"


// Set up nRF24L01 radio on SPI bus plus pins 9 & 10
RF24 radio(9,10);

payload p;

// Radio pipe addresses for the 2 nodes to communicate.
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };

typedef enum {send,sent,recived,no_response} state_e;

state_e state=send;
unsigned long time;
char str[17];


void setup()
{

	Serial.begin(57600);
	
	radio.begin();
	radio.setRetries(15,15);
	radio.setPayloadSize(17);
	radio.openWritingPipe(pipes[0]);
	radio.openReadingPipe(1,pipes[1]);
	radio.startListening();


	radio.printDetails();


p.chanel1=127;
p.chanel2=0;
p.button1=false;
p.button2=false;
}


// The loop function is called in an endless loop
void loop()
{
	switch(state){
	case send:{
		radio.stopListening();
		bool ok = radio.write( &p, sizeof(p));
		time = millis();
		if (ok){
			state=sent;
		}
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
		Serial.write(str);
    Serial.write("\n");
		state=send;
		break;
	}
	case no_response:{
		Serial.write("Not connected\n");
		state=send;
		break;
	}

	}//end switch
  if(Serial.available()){
    p.chanel1=Serial.parseInt();
    p.chanel2=Serial.parseInt();
    p.button1=Serial.parseInt()==1;
    p.button2=Serial.parseInt()==1;
  }

}

