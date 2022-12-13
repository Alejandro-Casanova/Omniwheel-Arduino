//********************************************************************************************************************
//***  Este archivo se encarga de gestionar las cadenas de texto y convertirlas a valores numericos ******************
//********************************************************************************************************************

//********************************************************************************************************************
// El formato de la cadena esperado es el siguiente: 
// Cadena: "x:000098y:100056z:000067"
// Donde el primer digito se corresponde con el signo y el resto a cada una de las coordenadas del vector
//********************************************************************************************************************

int ATDec(int ascii);                          //Convierte el valor en ASCII de la cadena a un numero decimal
void StringToVector(double* vector, char* string);
void StringToVector(int* vector, char* string);
void StringToGPIO(int* pin, int* val, char* string);
