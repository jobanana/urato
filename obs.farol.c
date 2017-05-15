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
	
	readAnalogSensors();	
	printf("\n leu os sensores");

	printf("Obst_left=%03d, Obst_center=%03d, Obst_right= 03d, Bat_voltage=%03d, Ground_sens=", analogSensors.obstSensLeft,analogSensors.obstSensFront, analogSensors.obstSensRight, analogSensors.batteryVoltage);

	senesq = analogSensors.obstSensLeft;
	sendir = analogSensors.obstSensRight;
	senfre = analogSensors.obstSensFront;

	setVel2(20,20);
	
	// Detec obst. Direita
	if( sendir > 350){
		setVel2(-15,60);
					
	}

	// Detec. obst. Esquerda	
	if(senesq > 350){
		setVel2(60,-15);
			
	}

	// Obstc. Frente
	if(senfre > 400){
		if(senesq > sendir){
			setVel2(60,-15);
		}
		else{
			setVel2(-15,60);
		}
	
	}

	printf("\n Fim de ciclo");

    } while(!stopButton());

    disableObstSens();
    }
    return 0;
}

void SrtScan(){
	while(!readBeaconSens()){
		setVel2(-50,50)
	}
	setVel2(0,0);
	delay(200,200);
	
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
    setVel2(80,-20);
    delay(700);
    setVel2(0,0);
}

void TurnLeft(){

    setVel2(-20,80);
    delay(700);
    setVel2(0,0);
}

void GoFrwd(){
     setVel2(20,20);
}

void GoBack(){
    setVel2(0,0);
    setVel2(-20,-20);
}

