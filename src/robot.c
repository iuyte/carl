#include "../include/robot.h"

Servo  *claw;
System *arm;
System *mogo;
System *drive[2];

Sensor *armCoder;
Sensor *driveCoder[2];
Sensor *mogoAngle;

void driveSet(int l, int r) {
	drive[0]->power = l;
	drive[1]->power = r;
} /* driveSet */
