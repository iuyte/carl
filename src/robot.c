#include "../include/robot.h"

Servo *claw;
Motors arm;
Motors mogo;
Motors drive;

void manager(void *none) {
	while (true) {
		arm.left->power   = arm.value;
		arm.right->power  = arm.value;
		mogo.left->power  = mogo.value;
		mogo.right->power = mogo.value;
		delay(5);
	}
} /* manager */

void driveSet(int l, int r) {
	drive.left->power  = l;
	drive.right->power = r;
} /* driveSet */
