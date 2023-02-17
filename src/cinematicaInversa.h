//*******************************************************************************************
//********  En este archivo se gestiona la cinematica inversa del robot   *******************
//Se trata de una adaptación del código generado en Matlab por:
//La adaptación ha sido realizada por Alberto López Rodríguez
//*******************************************************************************************

/*******************************************************************************************
************ Constantes*********************************************************************
*******************************************************************************************/

#include <Arduino.h>

#define ang_giro    360 //1.8
#define steps_rev   (360/ang_giro)
#define microsteps  4000 //256
#define f_ti       1.4
#define f_ajuste    1 
#define f_reductoras 10
#define f_ajuste_pasos  1.111//Factor de ajuste experimental para que recorra una distancia en el tiempo adecuado

#define L 0.27 //0.09 //0.29
#define R 0.063 //0.028 //0.063

#define PHI_1 (3.141592/2)
#define PHI_2 (-3.141592/6)
#define PHI_3 (3.141592*7/6)


//*******************************************************************************************
//*********Prototipos funciones
//*******************************************************************************************

//Definicion de las variables:
//Longitud: distancia entre la Rueda y el cdm del Robot
//Radio: radio de las ruedas.
//AngulosRueda: angulos de las ruedas respecto al sistema de referencia inicial.
//velocidad: [Vx,Vy,W] [m/s,m/s,rad/s]
//omega: Angulo que define la direccion del robot respecto del sistema inicial.
//w: vector que almacena las velocidades angulares de los motores
void cinematicaDirecta(float longitud, float radio, double* angulosRuedas,double* vel,double omega,int* w);
void cinematicaInversa(float longitud, float radio, double* angulosRuedas,double* vel,double omega,int* w);
void cinematicaInversaRadianes(float longitud, float radio, double* angulosRuedas,double* velocidad,double omega,double* w);
void relativePosition(int* posAct,double* posReq);
