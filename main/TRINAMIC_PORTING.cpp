

/*
 * TRINAMIC_PORTING.c
 *
 *  Created on: Nov 8, 2018
 *      Author: pasto
 */

#include "TRINAMIC_PORTING.h"

#include<SPI.h>

uint8_t motorPin[3];
void init_pins(){
  /*pinMode(DM1,OUTPUT);
  pinMode(DM2,OUTPUT);
  pinMode(DM3,OUTPUT);
  pinMode(DM4,OUTPUT);*/
  pinMode(MOT0,OUTPUT);
  pinMode(MOT1,OUTPUT);
  pinMode(MOT2,OUTPUT);
  
  //pinMode(MOT3,OUTPUT);

  //Enable de los drivers
  // pinMode(7, OUTPUT);
  // digitalWrite(7,LOW);

  //definir el CS del motor
  //placa nueva:
  /*
  motorPin[0]=6;
  motorPin[1]=7;
  motorPin[2]=10;
  */
  //placa vieja:
  // motorPin[0]=9;
  // motorPin[1]=8;
  // motorPin[2]=7;//bien
  
}
void initSPI(){
  SPI.begin();
}
void chipSelect(int numero){
  /*int s1,s2,s3,s4;
  
  switch(numero){
    case 0: //para desactivar el mux activamos una salida que no esta conectada
      s4=0;       
      s3=0;     
      s2=0;      
      s1=0;
      break;
    case 1:
      s4=0;       
      s3=0;     
      s2=0;      
      s1=1;
      break;
    case 2:
      s4=0;
      s3=0;
      s2=1;      
      s1=0;
      break;
    case 3:
      s4=0;       
      s3=0;     
      s2=1;      
      s1=1;
      break;
    case 4:
      s4=0;       
      s3=1;     
      s2=0;      
      s1=0;
      break;
    case 5:
      s4=0;       
      s3=1;     
      s2=0;      
      s1=1;
      break;
    case 6:
      s4=0;       
      s3=1;     
      s2=1;      
      s1=0;
      break;
    case 7:
      s4=0;       
      s3=1;     
      s2=1;      
      s1=1;
      break;
    case 8:
      s4=1;       
      s3=0;     
      s2=0;      
      s1=0;
      break;
    case 9:
      s4=1;       
      s3=0;     
      s2=0;      
      s1=1;
      break;
    case 10:
      s4=1;       
      s3=0;     
      s2=1;      
      s1=0;
      break;
    case 11:
      s4=1;
      s3=0;
      s2=1;
      s1=1;
      break;
    case 12:
      s4=1;
      s3=1;
      s2=0;
      s1=0;
    default:
      s4=1;
      s3=1;
      s2=1;
      s1=1;
      break;
  }

  //Actualizamos la entrada del mux
  if(s1==1){digitalWrite(DM1,HIGH);}
  else{digitalWrite(DM1,LOW);}


  if(s2==1){digitalWrite(DM2,HIGH);}
  else{digitalWrite(DM2,LOW);}

  if(s3==1){digitalWrite(DM3,HIGH);}
  else{digitalWrite(DM3,LOW);}

  if(s4==1){digitalWrite(DM4,HIGH);}
  else{digitalWrite(DM4,LOW);}
*/

switch (numero){
  case 0:
      digitalWrite(MOT0,LOW);
      digitalWrite(MOT1,HIGH);
      digitalWrite(MOT2,HIGH);
      digitalWrite(MOT3,HIGH);     
      break;
  case 1:
      digitalWrite(MOT0,HIGH);
      digitalWrite(MOT1,LOW);
      digitalWrite(MOT2,HIGH);
      digitalWrite(MOT3,HIGH);
      break;
  case 2:
      digitalWrite(MOT0,HIGH);
      digitalWrite(MOT1,HIGH);
      digitalWrite(MOT2,LOW);
      digitalWrite(MOT3,HIGH);
      break;
  case 3:
      digitalWrite(MOT0,HIGH);
      digitalWrite(MOT1,HIGH);
      digitalWrite(MOT2,HIGH);
      digitalWrite(MOT3,LOW);
      break;
  default:
      digitalWrite(MOT0,HIGH);
      digitalWrite(MOT1,HIGH);
      digitalWrite(MOT2,HIGH);
      digitalWrite(MOT3,HIGH);
      break;
}
  
}
// TMC4361A SPI wrapper
void tmc4361A_writeDatagram(uint8 motor, uint8 address, uint8 x1, uint8 x2, uint8 x3, uint8 x4)
{
    int value = x1;
  	value <<= 8;
  	value |= x2;
  	value <<= 8;
  	value |= x3;
  	value <<= 8;
  	value |= x4;

    tmc40bit_writeInt(motor, address, value);
}

void tmc4361A_writeInt(uint8 motor, uint8 address, int value)
{
    tmc40bit_writeInt(motor, address, value);
}

int tmc4361A_readInt(u8 motor, uint8 address)
{
    tmc40bit_readInt(motor, address);
    return tmc40bit_readInt(motor, address);
}

// General SPI decription
void tmc40bit_writeInt(u8 motor, uint8 address, int value)
{
    char tbuf[5];

    for (int i = 0; i < 5; i++)
    {
    	tbuf[i] = 0;
    }

    tbuf[0] = address | 0x80;
    tbuf[1] = 0xFF & (value>>24);
    tbuf[2] = 0xFF & (value>>16);
    tbuf[3] = 0xFF & (value>>8);
    tbuf[4] = 0xFF & value;


    chipSelect(motor);
    SPI.transfer(tbuf,5);
    chipSelect(4);
}

int tmc40bit_readInt(u8 motor, uint8 address)
{
    char tbuf[5], rbuf[5];
    int32_t value;
	// clear write bit
    value = 0;
    for (int i = 0; i < 5; i++)
    {
    	tbuf[i] = 0;
    	rbuf[i] = 0;
    }

	tbuf[0] = address & 0x7F;

    chipSelect(motor);
    for(int i=0;i<5;i++){
      rbuf[i]=SPI.transfer(tbuf[i]);
    }
    chipSelect(4);

	value =rbuf[1];
	value <<= 8;
	value |= rbuf[2];
	value <<= 8;
	value |= rbuf[3];
	value <<= 8;
	value |= rbuf[4];

  //Si el valor es negativo
  if(value>0x7FFFFF){
    //Completa los bits que le falta para estar en complemento a 2
    value|=0xFF000000;
  }
	return value;
}


int tmc40bit_readStatus(u8 motor, uint8 address)
{
    char tbuf[5], rbuf[5];
    int32_t value;
	// clear write bit
    value = 0;
    for (int i = 0; i < 5; i++)
    {
    	tbuf[i] = 0;
    	rbuf[i] = 0;
    }

	tbuf[0] = address & 0x7F;

    chipSelect(motor);
    for(int i=0;i<5;i++){
      rbuf[i]=SPI.transfer(tbuf[i]);
    }
    chipSelect(4);

	
	return rbuf[0];
}
