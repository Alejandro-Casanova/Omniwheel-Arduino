
#include "userMotorControl.h"
#include <Arduino.h>
#include "TMC4361A/TMC4361A.h"
#include "TRINAMIC_PORTING.h"
#include "cinematicaInversa.h"
#include <math.h>


double phis_mot[3]={PHI_1,PHI_3,PHI_2}; 

void setSpeeds(int* velMotores){

  controlSpeed(0,velMotores[0]);
  controlSpeed(1,velMotores[1]);
  controlSpeed(2,velMotores[2]);

}

void initMotors(){
// initialize all motors
  for(int i = 0; i < 3; i++)
  {

    // MULTISTEP_FILT=1, EN_PWM_MODE=1 enables stealthChop
    //tmc4361A_writeInt(i, TMC4361A_GENERAL_CONF, 0x0000000C);//4

    // TOFF=3, HSTRT=4, HEND=1, TBL=2, CHM=0 (spreadCycle)
    //tmc4361A_writeInt(i, TMC4361A_CHOPCONF, 0x000100C3);
    // // IHOLD=10, IRUN=15 (max. current), IHOLDDELAY=6
    // tmc4361A_writeInt(i, TMC4361A_IHOLD_IRUN, 0x00040905);

    // // TPOWERDOWN=10: Delay before power down in stand still
    // tmc4361A_writeInt(i, TMC4361A_TPOWERDOWN, 0x0000000A);

    // // TPWMTHRS=500
    // tmc4361A_writeInt(i, TMC4361A_TPWMTHRS, 0x000001F4);

    // // Values for speed and acceleration
    // tmc4361A_writeInt(i, TMC4361A_VSTART, 1);
    // tmc4361A_writeInt(i, TMC4361A_A1, 5000);
    // tmc4361A_writeInt(i, TMC4361A_V1, 26843);
    // tmc4361A_writeInt(i, TMC4361A_AMAX, 5000);
    // tmc4361A_writeInt(i, TMC4361A_VMAX, 50000);
    // tmc4361A_writeInt(i, TMC4361A_DMAX, 5000);
    // tmc4361A_writeInt(i, TMC4361A_D1, 5000);
    // tmc4361A_writeInt(i, TMC4361A_VSTOP, 10);
    // tmc4361A_writeInt(i, TMC4361A_RAMPMODE, TMC4361A_MODE_POSITION);
    // tmc4361A_writeInt(i, TMC4361A_XACTUAL,0);

    tmc4361A_writeInt(i, TMC4361A_CLK_FREQ, 24000000); //24 MHz (external clock provided oscillator)
    tmc4361A_writeInt(i, TMC4361A_STP_LENGTH_ADD, 99);
  }
  
  int ws[3]={0,0,0};
  setSpeeds(ws);
}

void controlSpeed(int targetMotor, int vel, double acc)
{

  if (vel == 0)
  {
    tmc40bit_writeDouble(targetMotor, TMC4361A_VMAX, 0.0, 8);
    return;
  }
  //uint8_t buff[100];
  //sprintf((char*)buff, "Motor %d -> req speed: %d",targetMotor, vel);
  //Serial.println((char*)buff);

  // Velocity Mode S-Shape Ramp
  tmc4361A_writeInt(targetMotor, TMC4361A_RAMPMODE, TMC4361A_RAMP_SSHAPE);

  // Set Bow Values
  tmc4361A_writeInt(targetMotor, TMC4361A_BOW1, 500);
  tmc4361A_writeInt(targetMotor, TMC4361A_BOW2, 500);
  tmc4361A_writeInt(targetMotor, TMC4361A_BOW3, 500);
  tmc4361A_writeInt(targetMotor, TMC4361A_BOW4, 500);

  // Set Max Acceleration and Deceleration
  tmc40bit_writeDouble(targetMotor, TMC4361A_AMAX, acc, 2);
  tmc40bit_writeDouble(targetMotor, TMC4361A_DMAX, acc, 2);

  // Set initial and final acceleration and deceleration
  tmc40bit_writeDouble(targetMotor, TMC4361A_ASTART, 0.0, 2);
  tmc40bit_writeDouble(targetMotor, TMC4361A_DFINAL, 0.0, 2);

  tmc40bit_writeDouble(targetMotor, TMC4361A_VMAX, (double)vel, 8); //*100

}

void controlPos(int targetMotor, int pasos, int vel){
    tmc4361A_writeInt(targetMotor, TMC4361A_RAMPMODE, TMC4361A_RAMP_TRAPEZ | TMC4361A_RAMP_POSITION);
    tmc40bit_writeDouble(targetMotor, TMC4361A_VMAX, (double)vel, 8);
    tmc4361A_writeInt(targetMotor, TMC4361A_XACTUAL, 0);
    tmc4361A_writeInt(targetMotor, TMC4361A_X_TARGET, pasos);
}

void controlPosition(int targetMotor, int pasos, int vel, int accel){
  if(vel<0){return;}
  if(accel<0){return;}

    tmc4361A_writeInt(targetMotor, TMC4361A_RAMPMODE, TMC4361A_RAMP_TRAPEZ | TMC4361A_RAMP_POSITION);
    tmc40bit_writeDouble(targetMotor, TMC4361A_VMAX, (double)vel, 8);
    tmc40bit_writeDouble(targetMotor, TMC4361A_AMAX, (double)accel, 2);
    tmc40bit_writeDouble(targetMotor, TMC4361A_DMAX, (double)accel, 2);
    tmc4361A_writeInt(targetMotor, TMC4361A_XACTUAL, 0);
    tmc4361A_writeInt(targetMotor, TMC4361A_X_TARGET, pasos);
}

void setRelPos(int* posRel,int tiempo){
  

  //Calculamos la distancia entre ambas posiciones
  double dist[3] = {0,0,0};

  dist[0] = posRel[0];
  dist[1] = posRel[1];
  dist[2] = posRel[2];

  //Calculamos las velocidades en cada eje en m/s
  double vels[3] = {0,0,0};

  vels[0] = dist[0]/tiempo;
  vels[1] = dist[1]/tiempo;
  vels[2] = dist[2]/tiempo;

  //Calculamos los pasos y velocidades en cada motor 
  int pasos[3]= {0,0,0};
  int w_mot[3] = {0,0,0};

  //Calculo velocidades motores
  cinematicaInversa(L,R,phis_mot,vels,0,w_mot);

  //Calculo de pasos
  for(int i=0;i<3;i++){
    pasos[i] = w_mot[i]*tiempo; //usteps
    //Ponemos las velocidades y los pasos en los drivers
    if(w_mot[i]<0){
      w_mot[i]=-w_mot[i]; //Como los pasos los ponemos negativos ponemos la velocidad positiva
    }
    controlPos(i,pasos[i], w_mot[i]);
  }

}

void setRelPosition(double* posRel,int max_w){

  //Calculamos el angulo en el que se mueve el robot
  double angle;
  angle = atan2(posRel[1],posRel[0]);


  /*Serial.println("angle");
  Serial.print(angle);*/
  //modulo vector
  double mod;
  mod = sqrt(posRel[1]*posRel[1]+posRel[0]*posRel[0]);

  if(mod==0){
    return;
  }
  //Vector unitario 1m/s en direccion angle
  double vel_unit[3] = {0,0,0};


  vel_unit[0] = posRel[0]/mod;
  vel_unit[1] = posRel[1]/mod;
  vel_unit[2] = 0;


  double pasos_ideal;
  double w_ideal;
  double w_prop[3];
  
  //Calculamos el numero de pasos que daria un motor para recorrer la distancia si esta situado perpendicular al movimiento
  pasos_ideal = ((mod/(2*PI*R))*microsteps*steps_rev);
  //Velocidad de rueda ideal
  w_ideal = 1/(R);
  //Vemos en la proporcion que se mueven los motores
  cinematicaInversaRadianes(L, R, phis_mot, vel_unit, 0, w_prop);
  /*Serial.print("w_prop:");
  Serial.print(w_prop[0]);
  Serial.print("\t");
  Serial.print(w_prop[1]);
  Serial.print("\t");
  Serial.print(w_prop[2]);
  Serial.println("");*/

  //numero de pasos 
  int pasos[3];
  for(int i=0;i<3;i++){
     w_prop[i] /= w_ideal;
     pasos[i] = w_prop[i]*pasos_ideal;

  }

  //Hacemos las velocidades positivas
   for(int i = 0; i < 3; i++){
    if(w_prop[i] < 0){
      w_prop[i] = -w_prop[i];
    }
  }

  double w_big = 1;
  //Escalamos teniendo en cuenta el maximo
  for(int i = 0; i < 3; i++){
    if( abs(w_prop[i]) > w_big ){
      w_big = abs(w_prop[i]);
    }
  }

  for(int i = 0; i < 3; i++){
    w_prop[i] = round( (w_prop[i] / w_big) * max_w);
  }

  /*Serial.println("pasos_ideal");
  Serial.println(pasos_ideal);

  Serial.print("modulo: ");
  Serial.println(mod);

  Serial.print("vel_unit: ");
  Serial.print(vel_unit[0]);
  Serial.print("\t");
  Serial.print(vel_unit[1]);
  Serial.print("\t");
  Serial.print(vel_unit[2]);
  Serial.println("");
  Serial.print("pasos:");
  Serial.print(pasos[0]);
  Serial.print("\t");
  Serial.print(pasos[1]);
  Serial.print("\t");
  Serial.print(pasos[2]);
  Serial.println("");
  Serial.print("w_prop_esc:");
  Serial.print(w_prop[0]);
  Serial.print("\t");
  Serial.print(w_prop[1]);
  Serial.print("\t");
  Serial.print(w_prop[2]);
  Serial.println("");*/

  
  for(int i=0;i<3;i++){
    controlPosition(i, pasos[i], w_prop[i], w_prop[i]/2);
  }




  /*//Calculamos la distancia entre ambas posiciones
  double dist[3] = {0,0,0};

  dist[0] = posRel[0];
  dist[1] = posRel[1];
  dist[2] = posRel[2];

  //Calculamos las velocidades en cada eje en m/s
  double vels[3] = {0,0,0};

  vels[0] = dist[0]/1; //Suponemos tiempo = 1s
  vels[1] = dist[1]/1;
  vels[2] = dist[2]/1;

  //Calculamos los pasos y velocidades en cada motor 
  int pasos[3]= {0,0,0};
  int w_mot[3] = {0,0,0};
  double w_esc[3] = {0,0,0};
  //Calculo velocidades motores
  cinematicaInversa(L,R,phis_mot,vels,0,w_mot);
  
  Serial.println("vels");
  Serial.print(vels[0]);
  Serial.print("\t");
  Serial.print(vels[1]);
  Serial.print("\t");
  Serial.print(vels[2]);
  Serial.println("");

  //Buscamos la velocidad w mas grande para referirla a la escala
  int w_max=1;

  for(int i=0;i<3;i++){
    if(abs(w_mot[i])>w_max){
      w_max=abs(w_mot[i]);
    }
  }

  //Escalamos las variables
  w_esc[0]=((double)w_mot[0]/w_max)*max_w;
  w_esc[1]=((double)w_mot[1]/w_max)*max_w;
  w_esc[2]=((double)w_mot[2]/w_max)*max_w;

  w_mot[0]=(w_esc[0]);
  w_mot[1]=(w_esc[1]);
  w_mot[2]=(w_esc[2]);


  Serial.println("w_mot");
  Serial.print(w_mot[0]);
  Serial.print("\t");
  Serial.print(w_mot[1]);
  Serial.print("\t");
  Serial.print(w_mot[2]);
  Serial.println("");


  //Calculamos el tiempo que tarda en llegar a la posicion a esas velocidades
  cinematicaDirecta(L,R,phis_mot,vels,0,w_mot);

  Serial.println("vels2");
  Serial.print(vels[0]);
  Serial.print("\t");
  Serial.print(vels[1]);
  Serial.print("\t");
  Serial.print(vels[2]);
  Serial.println("");

  double tiempo = 1;
  //Calculamos el tiempo
  for (int i = 0; i<3; i++){
     if((vels[i]!=0)&&(dist[i]!=0)){
       tiempo = dist[i]/vels[i];
     }
  }
  if(tiempo == 0){
    return;
  }

  Serial.println("tiempo");
  Serial.println(tiempo);

  //Calculos aceleraciones
  int accel[3]={0,0,0};
  for(int i = 0;i<3;i++){
      //suponiendo tiempo aceleracion 0.5s
      accel[i]= w_mot[i]*2;
  }
  

  if(tiempo<=0.5){
    tiempo+=0.5;
    accel[0]= 1500000;
    accel[1]= 1500000;
    accel[2]= 1500000;
  }

  //Calculo de pasos
  for(int i=0;i<3;i++){
    pasos[i] = w_mot[i]*(tiempo-0.5); //Como la aceleracion total es un segundo al tiempo total de desplazamiento hay que quitarle medio segundo
    
    //Ponemos las velocidades y los pasos en los drivers
    if(w_mot[i]<0){
      w_mot[i]=-w_mot[i]; //Como los pasos los ponemos negativos ponemos la velocidad positiva
      accel[i]=-accel[i]; 
    }

  }
      
  Serial.println("pasos");
  Serial.print(pasos[0]);
  Serial.print("\t");
  Serial.print(pasos[1]);
  Serial.print("\t");
  Serial.print(pasos[2]);
  Serial.println("");

  for(int i=0;i<3;i++){
    controlPosition(i,pasos[i], w_mot[i],accel[i]);
  }*/
}


//read vels in usteps/s
int readVel(int Motor){
   return tmc4361A_readInt(Motor, TMC4361A_VACTUAL); 
}



void readVels(double* vel){
  int w[3];
  double v[3] ={0,0,0};
  
  
  
  for (int i=0;i<3;i++){
    w[i] = readVel(i);
  }

    cinematicaDirecta(L,R,phis_mot,v,0,w);
    vel[0]=v[0];
    vel[1]=v[1];
    vel[2]=v[2];
}

int readPos(int Motor){
  
  return  tmc4361A_readInt(Motor, TMC4361A_XACTUAL); 
}

//Comvierte los upasos en metros
void readPositions(int* pos){
  for (int i=0;i<3;i++){
    pos[i] = readPos(i);
    pos[i]=(pos[i]/256)*(2*PI*R);
  }
}


void relCartesianPosition(double* posRel,int max_w){
  double conversion;
  double pasos[3];
  conversion = steps_rev*microsteps; //avance 1 mm y posicion en milimetros

  Serial.println("Relative Cartesian Pasos: ");
  for(int i=0;i<3;i++){
    pasos[i]=posRel[i]*conversion;
    Serial.println(pasos[i]);
    controlPosition(i,pasos[i], max_w, max_w);
  }
  
}

int positionReached(){
  char status_reg;
  for(int i = 0; i<3;i++){
    status_reg = tmc40bit_readStatus(i,TMC4361A_VACTUAL);
    if((status_reg&0x20)!=0x20){
      return 0;
    }
  }
  return 1;
}
