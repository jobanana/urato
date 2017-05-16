#include "mr32.h"
#include <math.h>
#include <string.h>
#include <stdlib.h>

    void TurnRight();
    void TurnLeft();
    void GoFrwd();
    void GoBack();
    void EyesOnTarget();
    int beaconScan();
		void moveBeaconToLight();
    void moveBeaconToOrigin(double,double);
    void obstacleAvoidance();
		int servoPos, count,sendir,senfre,senesq, contagem;
		double x,y,t;

int main(void)
{
  SERVO_WIDTH_MIN=975;
  SERVO_WIDTH_MAX=1955;
	initPIC32();
  SERVO_WIDTH_MIN=975;
  SERVO_WIDTH_MAX=1955;
	closedLoopControl( true );

	servoPos=0;
	count = 0;
	contagem=0;
	x=y=t=0;

  while(1)
  {
    while(!startButton());
    enableObstSens();
    moveBeaconToLight();
    wait(20);
    do
    {
      obstacleAvoidance();
      waitTick20ms();
      getRobotPos(&y,&x,&t);
      //printf("%f %f %f\n",-x,y,t);
      contagem+=1;
      if (contagem == 100) {
        moveBeaconToLight();
        contagem=0;
      }
    } while(readLineSensors(50)!=0x1F);
    leds(0xF);
    setVel2(0,0);
    wait(10);
      // sem espinha para cima
    getRobotPos(&y,&x,&t);
    moveBeaconToOrigin(-x,y);
    wait(10);
    contagem=0;
    do {
      obstacleAvoidance();
      getRobotPos(&y,&x,&t);
      waitTick20ms();
      contagem+=1;
      if (contagem == 100) {
        moveBeaconToOrigin(-x,y);
        contagem=0;
      }
    } while((sqrt(x*x+y*y)>100));
    setVel2(0,0);
    do{
      leds(0xF);
      wait(5);
      leds(0x0);
      wait(5);
    }while (1);
    }
    disableObstSens();
    return 0;
}
void moveBeaconToOrigin(double xi, double yi){
  double newangle=0;
  if (xi>0 && yi>0) { //1ºQ
    newangle=atan((yi)/(xi))+PI/2;
    leds(0x8);
  }
  if (xi>0 && yi<0) { //4ºQ
    newangle=atan(fabs(xi)/fabs(yi));
    leds(0x1);
  }
  if (xi<0 && yi<0) { //3ºQ
    newangle= -atan(xi/yi);
    leds(0x2);
  }
  if (xi<0 && yi>0) { //2ºQ
    newangle=-atan(fabs(yi)/fabs(xi))-PI/2;
    leds(0x4);
  }
    do{
      setVel2(20,-20);
      getRobotPos(&yi, &xi, &t);
    }while (fabs(newangle-t)>0.02);
    setVel2(0,0);
}
void moveBeaconToLight(){
  setServoPos(0);
    while (!readBeaconSens()) {
      setVel2(-40,40);
    }
    setVel2(0,0);
}
void obstacleAvoidance(){
  readAnalogSensors();
  senesq = analogSensors.obstSensLeft;
  sendir = analogSensors.obstSensRight;
  senfre = analogSensors.obstSensFront;
  setVel2(50,50);
  if( sendir > 350){
    setVel2(-25,60);
    }
  if(senesq > 350){
    setVel2(60,-25);
    }
  if(senfre > 400){
    if(senesq > sendir){
      setVel2(60,-15);
    }
    else{
      setVel2(-15,60);
      }
      if(abs(sendir - senfre) < 100){
        setVel2(-80,0);
      }
      if(abs(senesq - senfre) < 100){
        setVel2(0,-80);
      }
  }
}
