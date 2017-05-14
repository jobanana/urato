
#include "mr32.h"
#include <math.h>
#include <string.h>
#include <stdlib.h>


void rotateRel_naive(double deltaAngle);

int main(void)
{
	initPIC32();
  SERVO_WIDTH_MIN=945;
  SERVO_WIDTH_MAX=2000;
	closedLoopControl( true );
	setVel2(0, 0);
  while (!startButton());
  do {

  } while(|stopButton());

	return 0;
}
