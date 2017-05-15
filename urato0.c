
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
	SERVO_WIDTH_MIN=950;
  SERVO_WIDTH_MAX=1980;
	initPIC32();
  SERVO_WIDTH_MIN=950;
  SERVO_WIDTH_MAX=1980;
	closedLoopControl( true );
	setVel2(0, 0);

	servoPos=0;
  flagS = 1;
  count = 0;
	e_a=0;
  u_i_a=0;
  K=0.2;
  Ti=0.0001;
  Td=0.1;
  h=0.5;
  x=y=t=beaconangle=normangle=0;

	while (1) {
		while(!startButton());
    enableObstSens();
		servoPos=beaconScan();
		do
    {
			readAnalogSensors();
			getRobotPos(&x,&y,&t);
			tpos=t-t_a;
			servoPos=moveBeaconTo(servoPos,e);
			beaconangle=(double)servoPos*180/15;
			ypos= tpos*180/PI;
			e=beaconangle-ypos;
			u_p=K*e;
			u_d=K*Td*(e-e_a)/h;
			u_i=e*Ti*h/K + u_i_a;
			u=u_p+u_d/*+u_i*/;
			setVel2(50+u,50-u);
			u_i_a=u_i;
			e_a=e;
			t_a=t;
			if (abs(e)<5) {
				setVel2(50,50);
			}
			senesq = analogSensors.obstSensLeft;
			sendir = analogSensors.obstSensRight;
			senfre = analogSensors.obstSensFront;
			// obst. Esquerda
			if( sendir > 325){
				TurnLeft();
				setVel2(30,30);
			}
			// Obstc. Direita
			if(senesq > 325){
				TurnRight();
				setVel2(30,30);
			}
			// Obstc. Frente
			if(senfre > 325){
				setVel2(-25,90);
				delay(900);
				setVel2(30,30);
				}
		   } while(readLineSensors(50)!=0x1F);
			 leds(0xF);
	     setVel2(0,0);
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
  count=0;
  servoPos=-15;
  while (1) {
    if (servoPos==-15) {
      for (servoPos=-15; servoPos <15; servoPos+=1) {
        setServoPos(servoPos);
        delay(600);
        if (readBeaconSens()) {
          break;
        }
      }
    }
    if(servoPos==15){
      for (servoPos=15; servoPos >-15; servoPos-=1) {
        setServoPos(servoPos);
        delay(600);
        if (readBeaconSens()) {
          break;
        }
      }
    }
    if (servoPos<15 && servoPos>-15) {
      count=0;
      return servoPos;
      break;
    }
    else
    {
      count+=1;
    }
    if(count>=1)
    {
      setVel2(-50,50);
      wait(5);
      setVel2(0,0);
      count=0;
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
