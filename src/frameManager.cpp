
#include "frameManager.h"

int ATDec(int ascii){
  if(ascii>='0'&&ascii<='9'){
    return (ascii-'0');
  }

  return -1;
}

void StringToVector(int* vector, char* string){
  
  //Valor de x:
  if(string[1]=='0'){
    vector[0]=ATDec(string[2])*10000+ATDec(string[3])*1000+ATDec(string[4])*100+ATDec(string[5])*10+ATDec(string[6]);
  }
  else{
    vector[0]= - (ATDec(string[2])*1000+ATDec(string[3])*1000+ATDec(string[4])*100+ATDec(string[5])*10+ATDec(string[6]));
  }

  //Valor de y:
  if(string[9]=='0'){
    vector[1]=ATDec(string[10])*10000+ATDec(string[11])*1000+ATDec(string[12])*100+ATDec(string[13])*10+ATDec(string[14]);
  }
  else{
    vector[1]= - (ATDec(string[10])*10000+ATDec(string[11])*1000+ATDec(string[12])*100+ATDec(string[13])*10+ATDec(string[14]));
  }
  
  //Valor de z:
  if(string[17]=='0'){
    vector[2]=ATDec(string[18])*10000+ATDec(string[19])*1000+ATDec(string[20])*100+ATDec(string[21])*10+ATDec(string[22]);
  }
  else{
    vector[2]= - (ATDec(string[18])*10000+ATDec(string[19])*1000+ATDec(string[20])*100+ATDec(string[21])*10+ATDec(string[22]));
  }
  
}
void StringToVector(double* vector, char* string){
  
  //Valor de x:
  if(string[1]=='0'){
    vector[0]=ATDec(string[2])*10000+ATDec(string[3])*1000+ATDec(string[4])*100+ATDec(string[5])*10+ATDec(string[6]);
  }
  else{
    vector[0]= - (ATDec(string[2])*1000+ATDec(string[3])*1000+ATDec(string[4])*100+ATDec(string[5])*10+ATDec(string[6]));
  }

  //Valor de y:
  if(string[9]=='0'){
    vector[1]=ATDec(string[10])*10000+ATDec(string[11])*1000+ATDec(string[12])*100+ATDec(string[13])*10+ATDec(string[14]);
  }
  else{
    vector[1]= - (ATDec(string[10])*10000+ATDec(string[11])*1000+ATDec(string[12])*100+ATDec(string[13])*10+ATDec(string[14]));
  }
  
  //Valor de z:
  if(string[17]=='0'){
    vector[2]=ATDec(string[18])*10000+ATDec(string[19])*1000+ATDec(string[20])*100+ATDec(string[21])*10+ATDec(string[22]);
  }
  else{
    vector[2]= - (ATDec(string[18])*10000+ATDec(string[19])*1000+ATDec(string[20])*100+ATDec(string[21])*10+ATDec(string[22]));
  }
  
}
