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

double inch = (1 / (PI * (DRIVE_WHEEL_DIAMETER / 360) * DRIVE_ENCODER_RATIO));

Sensor armCoder;
Sensor liftCoder;
Sensor driveCoder[2];
Sensor mogoAngle[2];
Sensor gyro;
Sensor sonic;
Sensor clawAngle;
Sensor armLimit[2];

// Motors and servos
Motor claw;
Motor drive[2];
Motor arm[2];
Motor mogo[2];

// PID settings
PIDSettings armSettings;
PIDSettings driveSettings[2];

int selectedAuton = 1;

void altRefresh(Sensor *s) {
	mutexTake(s->mutex, -1);
	s->value = analogReadCalibrated(s->port);
	mutexGive(s->mutex);
}

void init();

void reset() {
	// Reset sensors
	sensorReset(&gyro);
	sensorReset(&driveCoder[0]);
	sensorReset(&driveCoder[1]);

	if (!isAutonomous()) {
		sensorReset(&armCoder);
		sensorReset(&mogoAngle[0]);
	}

	// Reset PID times
	armSettings.time      = millis();
	driveSettings[0].time = millis();
	driveSettings[1].time = millis();
} /* reset */

void update() {
	motorUpdate(&claw);
	motorUpdate(&mogo[0]);
	motorUpdate(&arm[0]);
	sensorRefresh(&armCoder);
	altRefresh(&mogoAngle[0]);
	altRefresh(&mogoAngle[1]);

	if (isAutonomous()) {
		sensorRefresh(&gyro);
		sensorRefresh(&sonic);
	}

	for (size_t i = 0; i < 2; i++) {
		motorUpdate(&drive[i]);
		sensorRefresh(&driveCoder[i]);
		sensorRefresh(&armLimit[i]);
	}
} /* update */

void info() {
	static unsigned long time = 0;
	if (millis() - time >= 25) {
		printf(
			RED " |  %4d     | " GREEN "%4d    | " YELLOW "%4d    | " \
			BLUE "%4d    | %4d    | " CYAN "%4d    | " RED "%3d    | " GREEN
			" %4d    | " \
			YELLOW "%4u mv | " RESET "\n",
			driveCoder[0].value,
			driveCoder[1].value,
			armCoder.value,
			mogoAngle[0].value,
			mogoAngle[1].value,
			0,
			gyro.average,
			sonic.value,
			powerLevelMain());
		lcdPrint(uart1, 2, "%u mV", powerLevelMain());
		time = millis();
	}
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
	// Call the init function to perform actions in init.c
	if (!initialized) {
		init();
	}
	reset();

	// Wait for initialization to end
	while (!isAutonomous() && !isEnabled()) {
		delay(15);
	}
} /* initialize */
