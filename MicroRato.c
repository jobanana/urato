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
    int onsigth, flagS, servoPos, count;
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
      servoPos=beaconScan();
      if (servoPos<15 && servoPos>-15) {
        setServoPos(servoPos);
        getRobotPos(&x,&y,&t);
        beaconangle=(double)servoPos*180/15;
        normangle=-(t*180/PI);
        printf("%f , %f \n",beaconangle, normangle);
        if (abs(beaconangle-normangle)<5) {
          setVel2(20,20);
          wait(10);
        }
        if(beaconangle+1<normangle)
        {
          setVel2(-8,8);
          setServoPos((servoPos+=1));
          wait(5);
        }else
        {
          setVel2(8,-8);
          setServoPos((servoPos-=1));
          wait(5);
        }
        count = 0;
      }else{
        count +=1;
      }
      if(count>2)
      {
        setVel2(-50,50);
        wait(5);
        setVel2(0,0);
        count=0;
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
