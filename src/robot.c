#include "../include/robot.h"

Sensor *arm;
Motor  *drive[2];

// Motor *lift;

void driveSet(int l, int r) {
	drive[0]->power = l;
	drive[1]->power = r;
} /* driveSet */
