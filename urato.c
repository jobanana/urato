
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
	int moveBeaconTo(int, double);
	int onsigth, flagS, servoPos, count,sendir,senfre,senesq;
	double x,y,t,tpos,t_a,h,beaconangle,normangle,ypos,u_p,u_d,u_i,u_i_a,u,e,e_a,K,Td,Ti;

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
	e_a=0;
  u_i_a=0;
  K=0.3;
  Ti=0.0001;
  Td=0.1;
  h=0.1;
  x=y=t=beaconangle=normangle=0;

	while (1) {
		while(!startButton());
    enableObstSens();
		servoPos=beaconScan();
		do
    {
			setVel2(30,30);
	  	waitTick40ms();
			readAnalogSensors();
			//printf("\n leu os sensores");
			senesq = analogSensors.obstSensLeft;
			sendir = analogSensors.obstSensRight;
			senfre = analogSensors.obstSensFront;
			// obst. Esquerda
			if( sendir > 325){
				//printf("if dir");
				//setVel2(50,-15); // Virar para a Direita
				TurnLeft();
				//delay(1000);
				setVel2(30,30);
			// necess�rio medir outra vez a dist�ncia ao obst�culo?
			}
			// Obstc. Direita
			if(senesq > 325){
				//printf("if Esquerda");
				//setVel2(-15,80); // Virar para a Esquerda
				TurnRight();
				//delay(1000);
				setVel2(30,30);
			//necess�rio medir outra vez a distancia do obstaculo
			}
			// Obstc. Frente
			if(senfre > 325){
				//printf("if frente");
				setVel2(-25,90);
				delay(900);
				setVel2(30,30);
				}
			//printf("\n Fim de ciclo");
		   } while(!stopButton());
		   disableObstSens();
		}
		return 0;
}
int moveBeaconTo(int servoPos, double e)
{

  while (!readBeaconSens()) {
    setVel2(0,0);
    leds(readBeaconSens());
    if (servoPos>14 || servoPos<-14) {
      servoPos=beaconScan();
      }
    if (e<0) {
      setServoPos((servoPos+=1));
      delay(500);
    }
    if (e>0) {
      setServoPos((servoPos-=1));
      delay(500);
    }
  }
  return servoPos;
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
    setVel2(80,-20);
    delay(3000);
    setVel2(0,0);
}
void TurnLeft(){

    setVel2(-20,80);
    delay(3000);
    setVel2(0,0);
}
void GoFrwd(){
     setVel2(20,20);
}
void GoBack(){
    setVel2(0,0);
    setVel2(-20,-20);
}
