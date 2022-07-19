//************************************************************************************************************************
//**********Archivo que contiene todas las funciones creadas para facilitar la comunicacion con los motores***************
//************************************************************************************************************************



//************************************************************************************************************************
//**********  Prototipos de las funciones  *******************************************************************************
//************************************************************************************************************************

void initMotors();                          //Funcion que inicializa los drivers de los motores y los pone a velocidad 0.
void controlSpeed(int targetMotor, int v);  //Funcion que permite controlar la velocidad de un motor. Ej: controlSpeed(0,5000);
void setSpeeds(int * velMotores);           //Funcion que permite escribir las velocidades de los tres motores. Ej: setSpeeds(velocidades);
int readVel(int Motor);                     //Funcion que lee la velocidad del motor
void readVels(double* v);
int readPos(int Motor);
void readPositions(int* pos);
void controlPos(int targetMotor, int pasos, int vel);
void controlPosition(int targetMotor, int pasos, int vel, int accel);
void setRelPos(int* posRe, int tiempo);
void setRelPosition(double* posRel,int max_w);
int positionReached();
void relCartesianPosition(double* posRel,int max_w);
