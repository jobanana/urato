#include "mr32.h"
#include <math.h>
#include <string.h>
#include <stdlib.h>

    void TurnRight();
    void TurnLeft();
    void GoFrwd();
    void GoBack();
    void SrtScan();
    void EyesOnTarget();
    int beaconScan();
    int onsigth, flagS, servoPos, count,sendir,senfre,senesq;
    double x,y,t,beaconangle,normangle;

int main(void)
{

int groundSensor;

  SERVO_WIDTH_MIN=975;
  SERVO_WIDTH_MAX=1955;
	initPIC32();
  SERVO_WIDTH_MIN=975;
  SERVO_WIDTH_MAX=1955;
closedLoopControl( true );

	setVel2(0, 0);

  servoPos=0;
  flagS = 1;
  count = 0;
  x=y=t=beaconangle=normangle=0;
 
  while(1)
  {
    while(!startButton());
    enableObstSens();
    do
    {
	
	setVel2(10,10);
	waitTick40ms();

	readAnalogSensors();	
	printf("\n leu os sensores");

	printf("Obst_left=%03d, Obst_center=%03d, Obst_right= 03d, Bat_voltage=%03d, Ground_sens=", analogSensors.obstSensLeft,analogSensors.obstSensFront, analogSensors.obstSensRight, analogSensors.batteryVoltage);

	senesq = analogSensors.obstSensLeft;
	sendir = analogSensors.obstSensRight;
	senfre = analogSensors.obstSensFront;


	// obst. Esquerda
	if(senesq < 200 && sendir > 300 && senfre < 200){
		printf("if dir");
		//setVel2(50,-15); // Virar para a Direita
		TurnRight();
		delay(1000);
		setVel2(10,10);
			
		
	// necessário medir outra vez a distância ao obstáculo?
	}
	// Obstc. Direita	
	if(senesq > 300 && sendir < 200 && senfre < 200){
		
		printf("if Esquerda");
		
		//setVel2(-15,50); // Virar para a Esquerda
		TurnLeft();
		delay(1000);
		setVel2(10,10);
		
	//necessário medir outra vez a distancia do obstaculo 
	}

	// Obstc. Frente
	if(senesq < 200 && sendir < 200 && senfre > 300){
			printf("if frente");
		if(servoPos < 15 && servoPos > 0 ){
			//setVel2(50,-15); // Virar para a Direita
			TurnRight();
		}
		else{
			//setVel2(-15,50); // Virar à Esquerda
			TurnLeft();
		}
		delay(1000);
		setVel2(10,10);
			
			}

	printf("\n Fim de ciclo");

    } while(!stopButton());

    disableObstSens();
    }
    return 0;
}

void SrtScan(){
  onsigth =  readBeaconSens();
  while(onsigth != 1){
    if (flagS==1) {
      setVel2(50,-50);
    }else
    {
      setVel2(-50,50);
    }
      onsigth = readBeaconSens();
  }
  flagS= !flagS;
}
int beaconScan() {
    if (servoPos==-15) {
      for (servoPos=-15; servoPos <15; servoPos+=1) {
        setServoPos(servoPos);
        delay(600);
        if (readBeaconSens()) {
          return servoPos;
        }
      }
    }
    if(servoPos==15){
      for (servoPos=15; servoPos >-15; servoPos-=1) {
        setServoPos(servoPos);
        delay(600);
        if (readBeaconSens()) {
          return servoPos;
        }
      }
    }
}

void TurnRight() {
    setVel2(75,-15);
    delay(800);
    setVel2(0,0);
}

void TurnLeft(){

    setVel2(-15,75);
    delay(800);
    setVel2(0,0);
}

void GoFrwd(){
     setVel2(20,20);
}

void GoBack(){
    setVel2(0,0);
    setVel2(-20,-20);
}
