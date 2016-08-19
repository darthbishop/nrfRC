/*
 * debug.h
 *
 *  Created on: 28 cze 2015
 *      Author: lech
 */

#ifndef DEBUG_H_
#define DEBUG_H_


void debug(String s){
#ifdef DEBUG
	Serial.write(s);
#endif
}




#endif /* DEBUG_H_ */
