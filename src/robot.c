/**
 * @file robot.c
 * @brief More general things related to the robot
 * Copyright (C) 2017 Ethan Wells
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or(at your option) any
 * later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty ofMERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <https://www.gnu.org/licenses/>
 */

#include "../include/robot.h"

Servo  *claw;
System *arm;
System *lift;
System *mogo;
System *drive[2];

Sensor *armCoder;
Sensor *liftCoder;
Sensor *driveCoder[2];
Sensor *mogoAngle;
Sensor *gyro[2];

TaskHandle managerHandle;

void init();

void driveSet(int l, int r) {
	drive[0]->power = l;
	drive[1]->power = r;
} /* driveSet */

void initialize() {
	// Clean up everything before we touch it. Otherwise, what we do next will be
	// overwritten
	sensorInit();
	motorInit();

	// Call the init function to do more stuff
	init();

	// Start the manager that manages everything basiccally
	managerHandle = taskCreate(&manager,
	                           TASK_DEFAULT_STACK_SIZE,
	                           NULL,
	                           TASK_PRIORITY_DEFAULT + 1);
} /* initialize */

void manager(void *none) {
	while (true) {
		motorLoop();
		sensorLoop();
		systemLoop();
		delay(5);
	}
	(void)none;
} /* manager */
