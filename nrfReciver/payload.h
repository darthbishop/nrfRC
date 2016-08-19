/*
 * payload.h
 *
 *  Created on: 28 cze 2015
 *      Author: lech
 */

#ifndef PAYLOAD_H_
#define PAYLOAD_H_

struct payload{
	byte chanel1,chanel2;
	bool button1,button2;
};

void readInputs(payload &p){
	p.chanel1=analogRead(A0)/4;
	p.chanel2=analogRead(A1)/4;

	if(digitalRead(A2)==LOW){
		p.button1=true;
	}else p.button1=false;

	if(digitalRead(A3)==LOW){
		p.button2=true;
	}else p.button2=false;
}


#endif /* PAYLOAD_H_ */
