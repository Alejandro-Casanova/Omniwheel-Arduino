
#include "frameManager.h"
#include <WString.h>
#include <Arduino.h>


int ATDec(int ascii){
  if(ascii>='0'&&ascii<='9'){
    return (ascii-'0');
  }

  return -1;
}

String getSubString(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void StringToVector(int* vector, char* string){
  
  String aux_string(string); 
  String part1 = getSubString(aux_string, ':', 0);
  String part2 = getSubString(aux_string, ':', 1);
  String part3 = getSubString(aux_string, ':', 2);

  // Serial.println("String Parts");
  // Serial.println(part1);
  // Serial.println(part2);
  // Serial.println(part3);

  // Valor de X
  if(part1[0] == '1'){
    vector[0] = -1;
    part1[0] = '0';
  }else{
    vector[0] = 1;
  }
  vector[0] *= part1.toInt();

  // Valor de Y
  if(part2[0] == '1'){
    vector[1] = -1;
    part2[0] = '0';
  }else{
    vector[1] = 1;
  }
  vector[1] *= part2.toInt();

  // Valor de Z
  if(part3[0] == '1'){
    vector[2] = -1;
    part3[0] = '0';
  }else{
    vector[2] = 1;
  }
  vector[2] *= part3.toInt();

  // //Valor de x:
  // if(string[1]=='0'){
  //   vector[0]=ATDec(string[2])*10000+ATDec(string[3])*1000+ATDec(string[4])*100+ATDec(string[5])*10+ATDec(string[6]);
  // }
  // else{
  //   vector[0]= - (ATDec(string[2])*1000+ATDec(string[3])*1000+ATDec(string[4])*100+ATDec(string[5])*10+ATDec(string[6]));
  // }

  // //Valor de y:
  // if(string[9]=='0'){
  //   vector[1]=ATDec(string[10])*10000+ATDec(string[11])*1000+ATDec(string[12])*100+ATDec(string[13])*10+ATDec(string[14]);
  // }
  // else{
  //   vector[1]= - (ATDec(string[10])*10000+ATDec(string[11])*1000+ATDec(string[12])*100+ATDec(string[13])*10+ATDec(string[14]));
  // }
  
  // //Valor de z:
  // if(string[17]=='0'){
  //   vector[2]=ATDec(string[18])*10000+ATDec(string[19])*1000+ATDec(string[20])*100+ATDec(string[21])*10+ATDec(string[22]);
  // }
  // else{
  //   vector[2]= - (ATDec(string[18])*10000+ATDec(string[19])*1000+ATDec(string[20])*100+ATDec(string[21])*10+ATDec(string[22]));
  // }
  
}
void StringToVector(double* vector, char* string){
  
  String aux_string(string); 
  String part1 = getSubString(aux_string, ':', 0);
  String part2 = getSubString(aux_string, ':', 1);
  String part3 = getSubString(aux_string, ':', 2);

  // Serial.println("String Parts");
  // Serial.println(part1);
  // Serial.println(part2);
  // Serial.println(part3);

  // Valor de X
  if(part1[0] == '1'){
    vector[0] = -1.0;
    part1[0] = '0';
  }else{
    vector[0] = 1.0;
  }
  vector[0] *= part1.toDouble();

  // Valor de Y
  if(part2[0] == '1'){
    vector[1] = -1.0;
    part2[0] = '0';
  }else{
    vector[1] = 1.0;
  }
  vector[1] *= part2.toDouble();

  // Valor de Z
  if(part3[0] == '1'){
    vector[2] = -1.0;
    part3[0] = '0';
  }else{
    vector[2] = 1.0;
  }
  vector[2] *= part3.toDouble();

  // //Valor de x:
  // if(string[1]=='0'){
  //   vector[0]=ATDec(string[2])*10000+ATDec(string[3])*1000+ATDec(string[4])*100+ATDec(string[5])*10+ATDec(string[6]);
  // }
  // else{
  //   vector[0]= - (ATDec(string[2])*1000+ATDec(string[3])*1000+ATDec(string[4])*100+ATDec(string[5])*10+ATDec(string[6]));
  // }

  // //Valor de y:
  // if(string[9]=='0'){
  //   vector[1]=ATDec(string[10])*10000+ATDec(string[11])*1000+ATDec(string[12])*100+ATDec(string[13])*10+ATDec(string[14]);
  // }
  // else{
  //   vector[1]= - (ATDec(string[10])*10000+ATDec(string[11])*1000+ATDec(string[12])*100+ATDec(string[13])*10+ATDec(string[14]));
  // }
  
  // //Valor de z:
  // if(string[17]=='0'){
  //   vector[2]=ATDec(string[18])*10000+ATDec(string[19])*1000+ATDec(string[20])*100+ATDec(string[21])*10+ATDec(string[22]);
  // }
  // else{
  //   vector[2]= - (ATDec(string[18])*10000+ATDec(string[19])*1000+ATDec(string[20])*100+ATDec(string[21])*10+ATDec(string[22]));
  // }
  
}
