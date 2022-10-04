#include "MatlabSocket.h"
#include <Arduino.h>

#include "arduino_secrets.h" 

#include "frameManager.h"
#include "userMotorControl.h"
#include "cinematicaInversa.h"


//Estos datos se encuentran en el archivo arduino secrets
char ssid[] = SECRET_SSID_4;    //Nombre del router
char pass[] = SECRET_PASS_4;    //Clave del Router

int status = WL_IDLE_STATUS;

uint8_t server[4] = {172, 16, 3, 130}; // Change to correct local ip
uint16_t port = 8090;

IPAddress ip(172, 16, 3, 131);  // Arduino static ip

double phis[3]={PHI_1,PHI_3,PHI_2}; 

//WiFiServer server(81); //Inicializamos un socket en el puerto 81
WiFiClient client;

boolean alreadyConnected = false; // Indica si el cliente estaba ya conectado


void initSocket(){


    //while (!Serial) {;} //Esperamos a que haya conexión serie

    //Comprueba si el modulo Wifi se encuentra disponible
    if (WiFi.status() == WL_NO_MODULE) {
      Serial.println("¡No se ha podido establecer comunicacion con el modulo Wifi!");
      //Bloqueamos la ejecución del programa
      while (true);
    }


    //Comprueba si la versión del firmware del Wifi está actualizada
    String fv = WiFi.firmwareVersion();
    if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
      Serial.println("Por favor, actualiza el firmware del dispositivo.");
      Serial.println("Wifi firmware"+fv+"Wifi last ver:"+WIFI_FIRMWARE_LATEST_VERSION);
    }

    // Set static ip
    WiFi.config(ip);

    // Intento de conexión a la red wifi:
    while (status != WL_CONNECTED) {
      Serial.print("Tratando de conectar al SSID: ");
      Serial.println(ssid);

      // Conectandose a la red WPA/WPA2:
      status = WiFi.begin(ssid, pass);

      if(status != WL_CONNECTED){
        Serial.print("No se pudo conectar al SSID: ");
        Serial.println(ssid);
        Serial.print("Tratando de conectar al SSID: ");
        Serial.println(ssid);
        
        //status = WiFi.begin(ssid,pass);
        
      }

      // if(status != WL_CONNECTED){
      //   Serial.print("No se pudo conectar al SSID: ");
      //   Serial.println(ssid_1);
      //   Serial.print("Tratando de conectar al SSID: ");
      //   Serial.println(ssid_2);
        
      //   status = WiFi.begin(ssid_2,pass_2);
        
      // }

      // if(status != WL_CONNECTED){
      //   Serial.print("No se pudo conectar al SSID: ");
      //   Serial.println(ssid_2);
      //   Serial.print("Tratando de conectar al SSID: ");
      //   Serial.println(ssid);
        
      //   status = WiFi.begin(ssid, pass);
        
      // }
      
      //Esperamos 5 segundos a que se realice la conexión
      delay(2000);
    }

  // Inicia el server:
  //server.begin();

  // Muestra los datos de la conexión:
  printWifiStatus();

  do{
    Serial.println("connecting to server...");
  }while( !client.connect(server, port) );

  Serial.println("Connection to server Successful!");
}

void printWifiStatus(){
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("Direccion IP: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("Intensidad (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
} 

const uint8_t battery_level = 55;

void send_status(){
  uint8_t infor[100];
  sprintf((char*)infor, "{\"msg_type\":\"status\",\"connection\":\"online\",\"battery\": \"%u\",\"time\": %lu}%c", battery_level, millis(), '\0');
  client.print((char*)infor);
  client.flush();
}

const char robot_name[] = "Omniwheel Test";
const char robot_type[] = "Omniwheel";

uint32_t timeout_counter = 0;
const int keepAlive_interval = 10000; // 10 s

void socketHandler(int * mensaje){
  //Espera a que se conecte un cliente:
  //WiFiClient client = server.available();


  //Cuando el Cliente escriba el primer byte saluda
  if (client.connected()) {
    uint8_t infor[100];
    // Serial.println("Connected to Server Now");
    // Serial.print("Status: ");
    // Serial.println(client.status());

    if(millis() - timeout_counter > keepAlive_interval){
      sprintf((char*)infor, "{\"msg_type\":\"keepAlive\"}%c", '\0');
      client.print((char*)infor);

      client.flush();

      timeout_counter = millis();
    }
    
    if (!alreadyConnected) {
      Serial.println("Just Connected!");
      //Limpia el buffer de entrada:
      client.flush();
      //Serial.println("Se ha establecido conexion con Matlab");
      alreadyConnected = true;

      sprintf((char*)infor, "{\"msg_type\":\"info\",\"name\": \"%s\",\"robot_type\": \"%s\",\"time\": %lu}", robot_name, robot_type, millis());
      client.print((char*)infor);

      client.flush();
      delay(500);
      send_status();

    }
    int i=1;
    if (client.available() > 0) {
      // Lee los bytes que lleguen del cliente:
      
      double speeds[3];
      int speeds_mot[3];
      double positions[3];
      static int posAct[3]= {0,0,0};
      static double velAct[3]= {0,0,0};
      static int tiempoAct = 0;
      static int tiempoPrev = 0;
      int posReached = 0;
      int ws[3]={0,0,0};

      client.read(infor,4);
      infor[4] = '\0';  
      //Serial.print((char*)infor);
      //MODO VELOCIDADES MOTORES "OWR:"
      if(infor[0]=='O'&&infor[1]=='W'&&infor[2]=='R'&&infor[3]==':'){
            
        client.read(infor,23);
        infor[23]='\0';
        //Serial.println((char*)infor);
        StringToVector(speeds_mot,(char*)infor);
        
        //sprintf((char*)infor, "speeds %d %d %d", speeds[0], speeds[1], speeds[2] );
        //Serial.println((char*)infor);
        setSpeeds(speeds_mot);
        
      }

      //NUEVOS MODOS "OWR_XX"
      else if(infor[0]=='O'&&infor[1]=='W'&&infor[2]=='R'&&infor[3]=='_'){
        
        client.read(infor,3);
        infor[3]='\0';
        //Serial.print((char*)infor);
        //Modo cinematica inversa "OWR_CI :: V_X :: V_Y :: V_W"
        if (infor[0]=='C'&&infor[1]=='I'&&infor[2]==':'){

          client.read(infor,23);
          infor[23]='\0';
          //Serial.println((char*)infor);

          StringToVector(speeds,(char*)infor);
          Serial.println(speeds[0]);

          //guardamos la velocidad actual
          /*velAct[0] = speeds[0];
          velAct[1] = speeds[1];
          velAct[2] = speeds[2];*/
          cinematicaInversa(L, R, phis, speeds, 0, ws);
          setSpeeds(ws);
        }
        else if (infor[0]=='R'&&infor[1]=='P'&&infor[2]==':' ){
            int tiemp_pos=0;
            client.read(infor,23);
            infor[23]='\0';
            //Serial.println((char*)infor);

            StringToVector(positions,(char*)infor);
            
            //tiemp_pos=positions[2];
            
            relativePosition(posAct,positions);

            /*if(tiemp_pos <=0){
              tiemp_pos=10;
            }*/
            posReached = 0;
            positions[0]/=1000;
            positions[1]/=1000;
            positions[2]/=1000;
            setRelPosition(positions,75000);
            //sprintf((char*)infor, "PosAct %d %d %d%c", posAct[0], posAct[1], posAct[2],'\0');
            //client.println((char*)infor);
            
        }
        else if(infor[0]=='C'&&infor[1]=='P'&&infor[2]==':' ){
          client.read(infor,23);
          infor[23]='\0';
          Serial.println((char*)infor);

          StringToVector(positions,(char*)infor);
          
          
          relativePosition(posAct,positions);

          
          relCartesianPosition(positions,50000);

        }
      
      }
      else if(infor[0]=='O'&&infor[1]=='W'&&infor[2]=='R'&&infor[3]=='-'){
        client.read(infor,5);
        infor[6]='\0';
        //Serial.print((char*)infor);

        if (infor[0]=='R'&&infor[1]=='V'&&infor[2]=='E'&&infor[3]=='L'&&infor[4]==':'){
          

          client.read(infor,23);
          infor[23]='\0';
          //Serial.println((char*)infor);
          
          //Guardamos el tiempo
          tiempoAct = millis();
          
          readVels(velAct);
          /* Serial.println(velAct[0]);
          Serial.println(velAct[1]);
          Serial.println(velAct[2]);*/

          //readPositions(posAct);

          


          posReached = positionReached();
          int velActString[3] ={round(velAct[0]),round(velAct[1]),round(velAct[2])};
          
          sprintf((char*)infor, "{\"vel\":[%d,%d,%d],\"pos\": [%i,%i,%i],\"tiempo\": %i,\"pos_reached\":%i}%c", velActString[0], velActString[1], velActString[2],posAct[0],posAct[1],posAct[2],tiempoAct,posReached,'\0');
          tiempoPrev = tiempoAct;
          //Serial.println((char*)infor);
          client.print((char*)infor);
            
            
        }
      }
      else{

        Serial.print("Mensaje no reconocido: ");
        Serial.write(infor, strlen((char*)infor));
        Serial.println("");
      }
      client.flush();
     
    }
    
  }else{ // Not connected
    Serial.println();
    Serial.println("Disconnected from Server.");
    client.stop();
    alreadyConnected = false;
    do{
      Serial.println("Trying to reconnect...");
    }while(!client.connect(server, port));
  }

  // if (client.available() == 0 ) {
  //   if(alreadyConnected){
  //     alreadyConnected=false;
  //     Serial.println("Client Disconnected\n");
  //     client.flush();
  //   }
  // }
}  
