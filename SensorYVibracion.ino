#include <NewPing.h>

#define tot 3 // total
#define usa 2
#define MAX_DISTANCE 200

//int triggerPin[tot] = {2,4,7};
//int echoPin[tot] = {3,5,6};
int salidaPin[tot]={13,12,11}; // Puertos usados para cada salida
int salidaEstado[tot] = {LOW,LOW,LOW};
unsigned long duracion[tot] = {0,0,0};
unsigned long ultimoMillis[tot] = {0,0,0};
unsigned long ultimoMillisSonar[tot] = {0,0,0};
unsigned long espacio = 100; // espacio entre pulsos

NewPing sonar[tot] = {NewPing(2,3,MAX_DISTANCE),NewPing(4,5,MAX_DISTANCE),NewPing(7,6,MAX_DISTANCE)};
bool sonarInverso[tot] = {false,true,false};
  
//int triggerPinActual;
//int echoPinActual;
bool sonarInversoActual;
int salidaPinActual;
int salidaEstadoActual;
unsigned long anteriorMillis;
unsigned long anteriorMillisSonar;
unsigned long duracionActual;
unsigned long delaySonar = 1000;

void setup() {  
  for(int i=0; i < usa; i++){ // Inicializando pines digitales a usar como salida
    pinMode(salidaPin[i],OUTPUT);
    //NewPing sonarActual(triggerPinActual, echoPinActual, MAX_DISTANCE);
    //sonar[i] = sonarActual;
  }
  Serial.begin(9600);
}

void loop() {
  unsigned long currentMillis = millis();
  for(int i=0; i < usa; i++){
    //triggerPinActual = triggerPin[i];
    //echoPinActual = echoPin[i];
    salidaPinActual = salidaPin[i];
    anteriorMillis = ultimoMillis[i];  
    anteriorMillisSonar = ultimoMillisSonar[i];  
    duracionActual = duracion[i];
    salidaEstadoActual = salidaEstado[i];

    NewPing sonarActual = sonar[i];
    sonarInversoActual = sonarInverso[i];
    
    if(currentMillis - anteriorMillisSonar > delaySonar){ 
      ultimoMillisSonar[i] = currentMillis;
      int uS = sonarActual.ping_median();    
      int distancia = uS / US_ROUNDTRIP_CM;
      Serial.print("Distancia Pin");Serial.print(i);Serial.print(": ");
      Serial.print(distancia);Serial.println("cm");
      if(!sonarInversoActual){ // detecta presencia
        if( distancia > 0 && distancia < 10 ){
          duracion[i] = 1500;
        }else if( distancia >=10 && distancia < 30 ){
          duracion[i] = 600;
        }else if( distancia >=30 && distancia < 60 ){
          duracion[i] = 200;
        }else{
          duracion[i] = 0;
        }
      }else{ // detecta ausencia
        if(distancia>=0 && distancia < 40){
          duracion[i] = 0;
        }else{
          duracion[i] = 600;
        }
      }
    }
    
    if(salidaEstadoActual == LOW){
      if(currentMillis - anteriorMillis > espacio && duracionActual > 0) {
        ultimoMillis[i] = currentMillis;
        salidaEstadoActual = HIGH;
        salidaEstado[i] = salidaEstadoActual;
        digitalWrite(salidaPinActual, salidaEstadoActual);
      }
    }else{
      if(currentMillis - anteriorMillis > duracionActual) {
        ultimoMillis[i] = currentMillis;
        salidaEstadoActual = LOW;
        salidaEstado[i] = salidaEstadoActual;
        digitalWrite(salidaPinActual, salidaEstadoActual);
      }
    }   
  }
}
