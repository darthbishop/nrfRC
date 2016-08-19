#include <Servo.h>

#include <nRF24L01.h>
//#include <printf.h>
#include <RF24.h>
#include <RF24_config.h>

#include <SPI.h>

#include "nRF24L01.h"
#include "RF24.h"
#include "debug.h"
#include "payload.h"
#include "l293d.h"
//coment to disable debuging
#define DEBUG
// Set up nRF24L01 radio on SPI bus plus pins 9 & 10
RF24 radio(9,10);

payload p;
int battery, val;

// Radio pipe addresses for the 2 nodes to communicate.
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };

typedef enum {send,sent,recived,no_response} state_e;

state_e state=send;
unsigned long time;
String str;
#define servoPin 3
Servo myservo;  // create servo object to control a servo

L293d motor(5,7,8);
L293d pompa(6,A2,A3);

#define batteryPin A0

float voltage;
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
  myservo.attach(servoPin);  // attaches the servo on pin 9 to the servo object
}


void loop()
{
	if ( radio.available() ){
		radio.read(&p,sizeof (p));
    time = millis();
    val = map(p.chanel1, 0, 255, 70, 110);     // scale it to use it with the servo (value between 0 and 180) 
    myservo.attach(servoPin);
    myservo.write(val);
    motor.go(p.button1,p.chanel2);
    pompa.go(true,p.button2*255);
    
  battery=analogRead(batteryPin);
  voltage=2.0*5.0*battery/1023;
	str=String(p.chanel1)+" "+String(p.chanel2)+" "+String(p.button1)+" "+String(p.button2)+" "+String(voltage);
	radio.stopListening();

	radio.write( str.c_str(), 17);
	Serial.print(F("Sent response.\n\r"));

	// Now, resume listening so we catch the next packets.
	radio.startListening();
	}
 if (millis()-time>250){
  myservo.write(90);
  motor.stop();
  pompa.stop();
  myservo.detach();
 }
}
