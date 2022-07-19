/*
 * TRINAMIC_PORTING.h
 *
 *  Created on: Nov 8, 2018
 *      Author: pasto
 */



#ifndef TRINAMIC_PORTING_H_
#define TRINAMIC_PORTING_H_

#include "helpers/API_Header.h"

/*//Defines
//Multiplexor
#define DM1 6
#define DM2 19
#define DM3 20
#define DM4 21
*/
#define RELEASE 0  

#define MOT0 2
#define MOT1 1
#define MOT2 0
#define MOT3 21 //A6

//arduino conf
void initSPI();
void init_pins();
void chipSelect(int numero);

// TMC4361A SPI wrapper
void tmc4361A_writeDatagram(uint8 motor, uint8 address, uint8 x1, uint8 x2, uint8 x3, uint8 x4);
void tmc4361A_writeInt(uint8 motor, uint8 address, int value);
int tmc4361A_readInt(u8 motor, uint8 address);

// General SPI functions
void tmc40bit_writeInt(u8 motor, uint8 address, int value);
int tmc40bit_readInt(u8 motor, uint8 address);
int tmc40bit_readStatus(u8 motor, uint8 address);


#endif /* TRINAMIC_PORTING_H_ */
