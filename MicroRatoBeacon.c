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
    int moveBeacon(int, double);
    int onsigth, flagS, servoPos, count;
    double x,y,t,h,beaconangle,normangle,ypos,u_p,u_d,u_i,u_i_a,u,e,e_a,K,Td,Ti;

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
  K=0.15;
  Ti=0.01;
  Td=0.001;
  h=0.2;
  x=y=t=beaconangle=normangle=0;
  while(1)
  {
    while(!startButton());
    enableObstSens();
    servoPos=beaconScan();
    do
    {
        getRobotPos(&x,&y,&t);
        beaconangle=(double)servoPos*180/15;
        ypos= t*180/PI;
        e=beaconangle-ypos;
        u_p=K*e;
        u_d=K*Td*(e-e_a)/h;
        u_i=e*Ti*h/K + u_i_a;
        u=u_p+u_d/*+u_i*/;
        setVel2(40+u,40-u);
        servoPos=moveBeaconTo(servoPos,e);
        printf("bp %d, e %f, u %f\n",servoPos,e,u);
        u_i_a=u_i;
        e_a=e;
        if (abs(e)<5) {
          setVel2(20,20);
        }
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
    if(count>1)
    {
      setVel2(-50,50);
      wait(5);
      setVel2(0,0);
      count=0;
    }
  }
}
int moveBeacon(int servoPos, double u)
{
    setServoPos((servoPos+=(int)u));
    return servoPos;
}

int moveBeaconTo(int servoPos, double e)
{

  while (!readBeaconSens()) {
    if (servoPos>14 || servoPos <-14) {
      printf("Erro Pos Servo\n");
      break;
    }
    if (e<0) {
      setServoPos((servoPos+=1));
      delay(800);
    }
    if (e>0) {
      setServoPos((servoPos-=1));
      delay(800);
    }
  }
  return servoPos;
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
