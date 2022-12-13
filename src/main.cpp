#include <Arduino.h>
#include <stdint.h>

//PRUEBA PLACA PORTENTA
//Librerias
#include "frameManager.h"
#include "TRINAMIC_PORTING.h"
#include "userMotorControl.h"
#include "MatlabSocket.h"
#include"cinematicaInversa.h"


#define VELBASE 0.5


int ws[3]={-40744,-40744,-40744};
//double vels[3]={0,0,M_PI/2};
int vels[3]={0,5,0};
char incomingByte = 0;
int choice = 0;
double piss[3]={3.141592/2,-3.141592/6,3.141592*7/6};

void motor_test(){
  Serial.println("LOOP");
  while(1){
    //clkGen_setup();
    Serial.println("LOOP");
    controlSpeed(0, 500000);
    controlSpeed(1, 500000);
    controlSpeed(2, 500000);
    delay(3000);
    controlSpeed(0, 0);
    controlSpeed(1, 0);
    controlSpeed(2, 0);
    delay(3000);
    controlSpeed(0, -500000);
    controlSpeed(1, -500000);
    controlSpeed(2, -500000);
    delay(3000);
    controlSpeed(0, 0);
    controlSpeed(1, 0);
    controlSpeed(2, 0);
    delay(3000);
  }
}

void clkGen_setup(){

  // Set the divisor for GCLK5.
  REG_GCLK_GENDIV = GCLK_GENDIV_DIV(1) |  // Set divisor to 1
                    GCLK_GENDIV_ID(4);    // For GCLK5  
  while(GCLK->STATUS.bit.SYNCBUSY);       // Wait for write synchronization

  // Set the clock source, duty cycle, and enable GCLK5  
  REG_GCLK_GENCTRL = GCLK_GENCTRL_SRC_DFLL48M | // Set 48MHz source
                    GCLK_GENCTRL_IDC |          // Improve Duty Cycle
                    GCLK_GENCTRL_GENEN |        // Enable GCLK
                    GCLK_GENCTRL_ID(4);         // For GLCK5    
  while(GCLK->STATUS.bit.SYNCBUSY);             // Wait for write synchronization

  // Route GLCK5 to TCC0 & TCC1, and enable the clock.
  REG_GCLK_CLKCTRL = GCLK_CLKCTRL_ID_TCC0_TCC1 | // Route GCLK5 to TCC0 & 1
                    GCLK_CLKCTRL_CLKEN |         // Enable the clock
                    GCLK_CLKCTRL_GEN_GCLK4;      // Select GCLK5
  while(GCLK->STATUS.bit.SYNCBUSY);              // Wait for write synchronization

  // Config Pins
  PORT->Group[g_APinDescription[7].ulPort]
    .PINCFG[g_APinDescription[7].ulPin].bit.PMUXEN = 1;
  PORT->Group[g_APinDescription[7].ulPort]
    .PMUX[g_APinDescription[7].ulPin >> 1].reg |= PORT_PMUX_PMUXO_F;
  PORT->Group[g_APinDescription[5].ulPort]
    .PINCFG[g_APinDescription[5].ulPin].bit.PMUXEN = 1;
  PORT->Group[g_APinDescription[5].ulPort]
    .PMUX[g_APinDescription[5].ulPin >> 1].reg |= PORT_PMUX_PMUXO_F;  

  // Config Prescaler and period 
  REG_TCC0_WAVE |= TCC_WAVE_WAVEGEN_NPWM;
  REG_TCC0_PER = 1; // Period = 1 -> To get 24 MHz
  REG_TCC0_CTRLA |= TCC_CTRLA_PRESCALER_DIV1
            | TCC_CTRLA_ENABLE;     // Requires SYNC on CTRLA
  while( TCC0->SYNCBUSY.bit.ENABLE
          || TCC0->SYNCBUSY.bit.WAVE
          || TCC0->SYNCBUSY.bit.PER ); // Wait for write synchronization

  // Config Duty Cycle
  uint16_t dutyCycle = map(50,0,100,0,2);

  // For pin 4
  REG_TCC0_CCB3 = dutyCycle;
  while(TCC0->SYNCBUSY.bit.CCB3); // Wait for write synchronization

  // For pin 5
  REG_TCC0_CCB1 = dutyCycle;
  while(TCC0->SYNCBUSY.bit.CCB1); // Wait for write synchronization

  Serial.print("Duty Cycle: ");
  Serial.print(dutyCycle);
  Serial.println("/2");
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  
  // WAIT FOR SERIAL
  // while(!Serial);
  //delay(4000);
  //delay(4000);
  //clkGen_setup();

  
  
  init_pins();
  initSPI();

  delay(2000); // IMPORTANT! MUST WAIT UNTIL DRIVERS ARE POWERED ON 

  initMotors(); 

  //delay(10000);
  initSocket();
  
}
int mensaje[24]={'0','0','0','0','0','0','0','0','0','0','0','0',
                 '0','0','0','0','0','0','0','0','0','0','0','0'};
int values[3]={-1,0,0};

void loop() {
  // motor_test();
  //setSpeeds(ws);
  /*ws[0]=0;
  ws[1]=0;
  ws[2]=0;*/
  
  socketHandler(mensaje);

  //chipSelect(1);
  //setRelPosition(values,50000);
  /*controlPos(0,200*256 ,50000);
  delay(5000);
  controlPos(0,-200*256,50000);
  delay(5000);*/
  //setRelPosition(values,5000);
  //controlPosition(0,2559997, 50000,50000);
  //delay(5000);
  /*cinematicaInversa(L, R, piss, values, 0, ws);
  
  setSpeeds(ws);
  readVels(ws);

 */
  //delay(500);
  //clkGen_setup();

}