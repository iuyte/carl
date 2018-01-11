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
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
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

double inch = (1 / (PI * (DRIVE_WHEEL_DIAMETER / 360) * (1 / DRIVE_ENCODER_RATIO)));

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
Motor arm;
Motor mogo;

// PID settings
PIDSettings armSettings = {
	DEFAULT_PID_SETTINGS,
	.kP = .7f,
	.kI = .17f,
	.kD = .08f,
	.root = &arm,
	.target = 10,
};

PIDSettings driveSettings[2] = {{
		DEFAULT_PID_SETTINGS,
	  .kP = .16f,
	  .kI = .043f,
	  .kD = .15f,
		.tolerance = 50,
		.root = &drive[0],
	}, {
		DEFAULT_PID_SETTINGS,
	  .kP = .16f,
	  .kI = .043f,
	  .kD = .15f,
		.tolerance = 50,
		.root = &drive[1],
	},
};

PIDSettings gyroSettings[2] = {{
		DEFAULT_PID_SETTINGS,
	  .kP = 3.441f,
	  .kI = 0.f,
	  .kD = 2.136f,
		.tolerance = 3,
		.precision = 270,
		.root = &drive[0],
		.sensor = &gyro,
	}, {
		DEFAULT_PID_SETTINGS,
	  .kP = -3.441f,
	  .kI = -0.f,
	  .kD = -2.136f,
		.tolerance = 3,
		.precision = 270,
		.root = &drive[1],
		.sensor = &gyro,
	},
};

void altRefresh(Sensor *s) {
	mutexTake(s->mutex, 5);
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
	armSettings._time      = millis();
	driveSettings[0]._time = millis();
	driveSettings[1]._time = millis();
} /* reset */

void update() {
	motorUpdate(&claw);
	motorUpdate(&mogo);
	motorUpdate(&arm);

	sensorRefresh(&armCoder);
	sensorRefresh(&clawAngle);
	sensorRefresh(&mogoAngle[0]);
	sensorRefresh(&mogoAngle[1]);

	if (true) { // isAutonomous()) {
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
	if (!mutexTake(drive[0]._mutex, 5)) {
		return;
	} else if (!mutexTake(drive[1]._mutex, 5)) {
		mutexGive(drive[0]._mutex);
		return;
	}

	drive[0].power = l;
	drive[1].power = r;

	for (int i = 0; i < 2; i++) {
		mutexGive(drive[i]._mutex);
		motorUpdate(&drive[i]);
	}
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
