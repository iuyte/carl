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

#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"

Sensor armCoder;
Sensor liftCoder;
Sensor driveCoder[2];
Sensor mogoAngle;
Sensor gyro;
Sensor sonic;
Sensor clawAngle;
Sensor armLimit;

// Motors and servos
Motor claw;
Motor drive[2];
Motor arm[2];
Motor mogo[2];

TaskHandle managerHandle;

void init();

void reset() {
	gyro.reset                  = true;
	mogoAngle.reset             = true;
	mogoAngle.redundancy->reset = true;
	driveCoder[0].reset         = true;
	driveCoder[1].reset         = true;
	armCoder.reset              = true;
	clawAngle.reset             = true;
} /* reset */

void info() {
	printf(
	  RED " |  %4f     | " GREEN "%4f    | " YELLOW "%4d    | "               \
	  BLUE "%4d    | " CYAN "%4d    | " RED "%3d    | " GREEN " %4d    | " \
	  YELLOW "%4u mv | " RESET "\n",
	  (float)(driveCoder[0].value / inch),
	  (float)(driveCoder[1].value / inch),
	  armCoder.value,
	  mogoAngle.value,
	  clawAngle.value,
	  gyro.value,
	  sonic.value,
	  powerLevelMain());
	lcdPrint(uart1, 2, "%u mV", powerLevelMain());
} /* info */

bool initialized = false;

void driveSet(int l, int r) {
	if (!mutexTake(drive[0].mutex, 5)) {
		return;
	} else if (!mutexTake(drive[1].mutex, 5)) {
		mutexGive(drive[0].mutex);
		return;
	}

	drive[0].power = l;
	drive[1].power = r;
	mutexGive(drive[0].mutex);
	mutexGive(drive[1].mutex);
} /* driveSet */

void initialize() {
	// Clean up everything before we touch it. Otherwise, what we do next will be
	// overwritten
	sensorInit();

	// Call the init function to perform actions in init.c
	if (!initialized) {
		init();
	}
	reset();

	// Start the manager that manages everything basiccally
	managerHandle = taskCreate(&manager,
	                           TASK_DEFAULT_STACK_SIZE,
	                           NULL,
	                           TASK_PRIORITY_DEFAULT + 1);

	// Wait for initialization to end
	while (!isAutonomous() && !isEnabled()) {
		delay(15);
	}
} /* initialize */

void manager(void *none) {
	while (true) {
		sensorLoop();
		info();
		delay(10);
	}
} /* manager */
