//********************************************************************************************
//**  En este archivo se implementa todo lo relativo a las funciones encargadas de crear  ****
//**  un socket TCP que habilitará la comunicación con la GUI de Matlab                   ****
//**  Parte de este codigo procede del ejemplo de la libreria WiFiNINA.h del ejemplo:     ****
//**  WiFiChatServer.ino                                                                  ****
//********************************************************************************************
#ifndef MATLAB_SOCKET_H_ARDUINO
#define MATLAB_SOCKET_H_ARDUINO

#include <WiFiNINA.h>


//********************************************************************************************
//**  Prototipos de funciones ****************************************************************
//********************************************************************************************
void initSocket();      //Se encarga de inicializar la conexion
void socketHandler(int * mensaje);   //Se encarga de manejar el envio y recepcion de mensajes
void printWifiStatus(); //Imprime por serial el estado de la conexion wifi


#endif