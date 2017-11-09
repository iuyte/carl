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

Motor claw;
Motor drive[2];
Motor arm[2];
Motor mogo[2];

Sensor armCoder;
Sensor liftCoder;
Sensor driveCoder[2];
Sensor mogoAngle;
Sensor clawAngle;
Sensor gyro;
Sensor sonic;
Sensor armLimit;

System Drive[2];
System Arm;
System Mogo;

TaskHandle managerHandle;

void init();
void initLoop();
void info() {
	printf(
	  RED " |  %4f     | " GREEN "%4f    | " YELLOW "%4ld    | "               \
	  BLUE "%4ld    | " CYAN "%4ld    | " RED "%3ld    | " GREEN " %4ld    | " \
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
	motorInit();

	// Call the init function to perform actions in init.c
	if (!initialized) {
		init();
	}
	initLoop();

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
		motorLoop();
		sensorLoop();
		info();
		delay(10);
	}
} /* manager */
