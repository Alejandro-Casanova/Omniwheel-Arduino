
#include"cinematicaInversa.h"
#include <math.h>



double phiis[3]={PHI_1,PHI_3,PHI_2}; 
void cinematicaDirecta(float longitud, float radio, double* angulosRuedas,double* vel,double omega,int* w){
  double detM;
  double M[9]={0,0,0,0,0,0,0,0,0};  //Componentes de la matriz geométrica.
  double MA[9]={0,0,0,0,0,0,0,0,0}; //Componentes de la matriz adjunta de M
  double MI[9]={0,0,0,0,0,0,0,0,0}; //Componentes de la matriz geométrica invertida

  
  // Conversion de pasos a velocidades angulares
  // Variables de velocidad (rad/s <- rev/s <- steps/s <- microsteps/s]
  w[0] = w[0]*(1000*(2*M_PI)/(steps_rev*microsteps*f_ti));
  w[1] = w[1]*(1000*(2*M_PI)/(steps_rev*microsteps*f_ti)); 
  w[2] = w[2]*(1000*(2*M_PI)/(steps_rev*microsteps*f_ti));

  //Calculo de la matriz geométrica
  //  M = 1/r*[ -sin(phi(1)),cos(phi(1)),L;       M =   [ M[0]  M[1]  M[2]
  //            -sin(phi(2)),cos(phi(2)),L;               M[3]  M[4]  M[5]
  //            -sin(phi(3)),cos(phi(3)),L];              M[6]  M[7]  M[8] ]

 // Serial.begin(9600);


  M[0] = (1 / radio)*(-sin(angulosRuedas[0]+omega));
  M[1] = (1 / radio)*( cos(angulosRuedas[0]+omega));
  M[2] = (1 / radio)*(longitud);

  M[3] = (1 / radio)*(-sin(angulosRuedas[1]+omega));
  M[4] = (1 / radio)*( cos(angulosRuedas[1]+omega));
  M[5] = (1 / radio)*(longitud);

  M[6] = (1 / radio)*(-sin(angulosRuedas[2]+omega));
  M[7] = (1 / radio)*( cos(angulosRuedas[2]+omega));
  M[8] = (1 / radio)*(longitud);

 

  //Calculo de la matriz inversa de M, MI [Metodo de la matriz adjunta]:
  //Determinante de la matriz M:
  detM = M[0]*M[4]*M[8]+M[3]*M[7]*M[2]+M[1]*M[5]*M[6]-(M[2]*M[4]*M[6]+M[5]*M[7]*M[0]+M[1]*M[3]*M[8]);
  
  //Calculo matriz adjunta
  MA[0] =  (M[4]*M[8]-M[5]*M[7]);
  MA[1] = -(M[3]*M[8]-M[5]*M[6]);
  MA[2] =  (M[3]*M[7]-M[4]*M[6]);

  MA[3] = -(M[1]*M[8]-M[2]*M[7]);
  MA[4] =  (M[0]*M[8]-M[2]*M[6]);
  MA[5] = -(M[0]*M[7]-M[1]*M[6]);

  MA[6] =  (M[1]*M[5]-M[2]*M[4]);
  MA[7] = -(M[0]*M[5]-M[2]*M[3]);
  MA[8] =  (M[0]*M[4]-M[1]*M[3]);

  
  //Cálculo de la matriz inversa
  MI[0] = (1/detM) * MA[0];
  MI[1] = (1/detM) * MA[3];
  MI[2] = (1/detM) * MA[6];

  MI[3] = (1/detM) * MA[1];
  MI[4] = (1/detM) * MA[4];
  MI[5] = (1/detM) * MA[7];

  MI[6] = (1/detM) * MA[2];
  MI[7] = (1/detM) * MA[5];
  MI[8] = (1/detM) * MA[8];
  
  //Determinación de las velocidades angulares
  vel[0] = round((MI[0]*w[0] + MI[1]*w[1] + MI[2]*w[2]));
  vel[1] = round((MI[3]*w[0] + MI[4]*w[1] + MI[5]*w[2]));
  vel[2] = round((MI[6]*w[0] + MI[7]*w[1] + MI[8]*w[2]));

 
}
void cinematicaInversa(float longitud, float radio, double* angulosRuedas,double* vel,double omega,int* w){
  
  double detM;
  double velocidad[3];
  double M[9]={0,0,0,0,0,0,0,0,0};  //Componentes de la matriz geométrica.


  //Calculo de la matriz geométrica
  //  M = 1/r*[ -sin(phi(1)),cos(phi(1)),L;       M =   [ M[0]  M[1]  M[2]
  //            -sin(phi(2)),cos(phi(2)),L;               M[3]  M[4]  M[5]
  //            -sin(phi(3)),cos(phi(3)),L];              M[6]  M[7]  M[8] ]

  velocidad[0] = (double)(vel[0]);
  velocidad[1] = (double)(vel[1]);
  velocidad[2] = (double)(vel[2]);

  M[0] = (1 / radio)*(-sin(angulosRuedas[0]+omega));
  M[1] = (1 / radio)*( cos(angulosRuedas[0]+omega));
  M[2] = (1 / radio)*(longitud);

  M[3] = (1 / radio)*(-sin(angulosRuedas[1]+omega));
  M[4] = (1 / radio)*( cos(angulosRuedas[1]+omega));
  M[5] = (1 / radio)*(longitud);

  M[6] = (1 / radio)*(-sin(angulosRuedas[2]+omega));
  M[7] = (1 / radio)*( cos(angulosRuedas[2]+omega));
  M[8] = (1 / radio)*(longitud);


  //Determinación de las velocidades angulares
  w[0] = M[0]*velocidad[0] + M[1]*velocidad[1] + M[2]*velocidad[2];
  w[1] = M[3]*velocidad[0] + M[4]*velocidad[1] + M[5]*velocidad[2];
  w[2] = M[6]*velocidad[0] + M[7]*velocidad[1] + M[8]*velocidad[2];


  // Conversion velocidades angulares a pasos 
  // Variables de velocidad (rad/s -> rev/s -> steps/s -> microsteps/s]
  w[0] = round((w[0]/(2*M_PI)*steps_rev*microsteps*f_ti)/1000);
  w[1] = round((w[1]/(2*M_PI)*steps_rev*microsteps*f_ti)/1000);
  w[2] = round((w[2]/(2*M_PI)*steps_rev*microsteps*f_ti)/1000);

}

void cinematicaInversaRadianes(float longitud, float radio, double* angulosRuedas,double* velocidad,double omega,double* w){
  
  double detM;
  double M[9]={0,0,0,0,0,0,0,0,0};  //Componentes de la matriz geométrica.


  //Calculo de la matriz geométrica
  //  M = 1/r*[ -sin(phi(1)),cos(phi(1)),L;       M =   [ M[0]  M[1]  M[2]
  //            -sin(phi(2)),cos(phi(2)),L;               M[3]  M[4]  M[5]
  //            -sin(phi(3)),cos(phi(3)),L];              M[6]  M[7]  M[8] ]


  M[0] = (1 / radio)*(-sin(angulosRuedas[0]+omega));
  M[1] = (1 / radio)*( cos(angulosRuedas[0]+omega));
  M[2] = (1 / radio)*(longitud);

  M[3] = (1 / radio)*(-sin(angulosRuedas[1]+omega));
  M[4] = (1 / radio)*( cos(angulosRuedas[1]+omega));
  M[5] = (1 / radio)*(longitud);

  M[6] = (1 / radio)*(-sin(angulosRuedas[2]+omega));
  M[7] = (1 / radio)*( cos(angulosRuedas[2]+omega));
  M[8] = (1 / radio)*(longitud);


  //Determinación de las velocidades angulares
  w[0] = M[0]*velocidad[0] + M[1]*velocidad[1] + M[2]*velocidad[2];
  w[1] = M[3]*velocidad[0] + M[4]*velocidad[1] + M[5]*velocidad[2];
  w[2] = M[6]*velocidad[0] + M[7]*velocidad[1] + M[8]*velocidad[2];

}


void relativePosition(int* posAct,double* posReq){
  posAct[0]= posReq[0]+posAct[0];
  posAct[1]= posReq[1]+posAct[1];
  posAct[2]= posReq[2]+posAct[2];
}
