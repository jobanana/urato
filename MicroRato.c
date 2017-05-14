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

	senesq = analogSensors.obstSensLeft;
	sendir = analogSensors.obstSensRight;
	senfre = analogSensors.obstSensFront;


	// obst. Esquerda
	if(senesq < 300 && sendir > 500 && senfre < 400){
		printf("if dir");
		setVel2(50,-15); // Virar para a Direita
		delay(1000);
		setVel2(10,10);
			
		break;
	// necess�rio medir outra vez a dist�ncia ao obst�culo?
	}
	// Obstc. Direita	
	if(senesq > 500 && sendir < 300 && senfre < 400){
		
		printf("if Esquerda");
		
		setVel2(-15,50); // Virar para a Esquerda
		delay(1000);
		setVel2(10,10);
		break;
	//necess�rio medir outra vez a distancia do obstaculo 
	}

	// Obstc. Frente
	if(senesq < 400 && sendir < 400 && senfre > 500){
			printf("if frente");
		if(servoPos < 15 && servoPos > 0 ){
			setVel2(50,-15); // Virar para a Direita
		}
		else{
			setVel2(-15,50); // Virar � Esquerda
		}
		delay(1000);
		setVel2(10,10);
			
		break;
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
    delay(500);
    setVel2(0,0);
}

void TurnLeft(){

    setVel2(-15,75);
    delay(500);
    setVel2(0,0);
}

void GoFrwd(){
     setVel2(20,20);
}

void GoBack(){
    setVel2(0,0);
    setVel2(-20,-20);
}
