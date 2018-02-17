/**
 * @file auto.c
 * @brief The primary source for the autonomous operation period
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

#include "../include/auto.h"

void autonLeftRed12();
void autonLeftRed22();
void autonRightRed12();
void autonRightRed22();
void autonLeftBlue12();
void autonLeftBlue22();
void autonSkills();
void autonTest();

void autonNone() {}

void testMotors();

int   selectedAuton         = 0;
Auton autons[MAX_AUTON + 1] =
{ {
		.name    = "< red left 12 >",
		.sensorName = "arm",
		.sensor  = &arm.sensor,
		.execute = &autonLeftRed12,
	},{
		.name    = "< red left 22  >",
		.sensorName = "gyr2",
		.sensor  = &gyro.child,
		.execute = &autonLeftRed22,
	},{
		.name    = "< red right 12 >",
		.sensorName = "claw",
		.sensor  = &claw.sensor,
		.execute = &autonRightRed12,
	},{
		.name    = "< red right 22 >",
		.sensorName = "mgo",
		.sensor  = &mogo.sensor,
		.execute = &autonRightRed22,
	},{
		.name    = "< blue left 12 >",
		.sensorName = "mgo",
		.sensor  = &mogo.sensor,
		.execute = &autonLeftRed12,
	},{
		.name    = "< blue left 22  >",
		.sensorName = "mgo",
		.sensor  = &mogo.sensor,
		.execute = &autonLeftRed22,
	},{
		.name    = "< blue right 12>",
		.sensorName = "mgo",
		.sensor  = &mogo.sensor,
		.execute = &autonRightRed12,
	},{
		.name    = "< blue right 22 >",
		.sensorName = "mgo",
		.sensor  = &mogo.sensor,
		.execute = &autonRightRed22,
	},{
		.name    = "<    skills    >",
		.sensorName = "mgo",
		.sensor  = &mogo.sensor,
		.execute = &autonSkills,
	},{
		.name    = "<     none     >",
		.sensorName = "mgo",
		.sensor  = &arm.sensor,
		.execute = &autonNone,
	},{
		.name    = "<     test     >",
		.sensorName = "lef",
		.sensor  = &drive[0].sensor,
		.execute = &autonTest,
	},{
		.name    = "< test motors  >",
		.sensorName = "rit",
		.sensor  = &drive[1].sensor,
		.execute = &testMotors,
	}, };

void armToPosition(float pos, unsigned long until) {
	armSettings.target = pos;
	until             += millis();

	do {
		PID(&armSettings);
		update();
		delay(10);
	} while (!armSettings.isTargetReached && millis() < until);
} /* armToPosition */

void driveToPosition(int l, int r, unsigned long until) {
	driveSettings[0].target = l;
	driveSettings[1].target = r;
	until                  += millis();

	do {
		PID(&driveSettings[0]);
		PID(&driveSettings[1]);
		PID(&armSettings);

		update();
		delay(10);
	} while ((!driveSettings[0].isTargetReached ||
	          !driveSettings[1].isTargetReached) &&
	         millis() < until);

	drive[0].power = 0;
	drive[1].power = 0;
	update();
} /* driveToPosition */

void driveToPositionAngle(int l, int r, int a, unsigned long until) {
	driveSettings[0].target = l;
	driveSettings[1].target = r;
	until                  += millis();

	do {
		PID(&driveSettings[0]);
		PID(&driveSettings[1]);
		PID(&armSettings);
		drive[0].power += (a - gyro.average) * 1.5;
		drive[1].power -= (a - gyro.average) * 1.5;

		update();
		delay(10);
	} while ((!driveSettings[0].isTargetReached ||
	          !driveSettings[1].isTargetReached) &&
	         millis() < until);

	drive[0].power = 0;
	drive[1].power = 0;
	update();
} /* driveToPosition */

void mogoP(int p) {
	while (isAutonomous() && abs(p - mogo.sensor->average) > 75) {
		mogo.power = sgn(p - mogo.sensor->average) * 127;
		PID(&armSettings);
		update();
	}

	mogo.power = 0;
	motorUpdate(&mogo);
} /* mogoP */

Task mogoPT(void *p) {
	mogoP((int)p);
} /* mogoPT */

float gyroPIDC[3] = {
	5.279,
	0.0,
	1.953,
};

void gyroPID(int target, int precision) {
	int error      = 0;
	int integral   = 0;
	int derivative = 0;

	do {
		derivative = (target - gyro.average) - error;
		error      = target - gyro.average;
		integral  += error / 10;

		drive[0].power = -((error * gyroPIDC[0]) +
		                   (integral * gyroPIDC[1]) +
		                   (derivative * gyroPIDC[2]));
		drive[1].power = ((error * gyroPIDC[0]) +
		                  (integral * gyroPIDC[1]) +
		                  (derivative * gyroPIDC[2]));

		if (lcdReadButtons(uart1)) {
			gyroPIDC[0] += 0.005;
			delay(100);
		}
		info();
		printf("%f\n", gyroPIDC[0]);
		lcdPrint(uart1, 1, "%f", ((float)(powerLevelMain())) / 1000.0);
		lcdPrint(uart1, 2, "%f", gyroPIDC[0]);
		update();
		delay(10);
	} while (true || abs(error) > precision ||
	         integral > 10);

	drive[0].power = 0;
	drive[1].power = 0;
} /* gyroPID */

void turnTo(int angle, unsigned long until) {
	until += millis();

	gyroSettings[0].target = angle;
	gyroSettings[1].target = angle;

	do {
		PID(&gyroSettings[0]);
		PID(&gyroSettings[1]);

		update();
		delay(10);
	} while ((!gyroSettings[0].isTargetReached ||
	          !gyroSettings[1].isTargetReached) &&
	         millis() < until);

	driveSet(0, 0);
} /* turnTo */

void getMogo() {
	claw.power         = -50;
	armSettings.target = ARM_QUARTER;
	PID(&armSettings);
	motorUpdate(&arm);
	delay(100);

	GO(mogoPT, MOGO_DOWN + 10);
	delay(750);

	driveToPosition(2350, 2350, 2100);

	TaskHandle mogoUpHandle = GO(mogoPT, MOGO_UP);
	driveToPosition(2600, 2600, 525);

	while (taskGetState(mogoUpHandle) != TASK_DEAD) {
		delay(10);
	}
} /* getMogo */

Task backUp(void *time) {
	unsigned long t = (unsigned long)time;

	while (millis() - t < 14250) {
		delay(10);
	}

	t = millis();

	while (isAutonomous() && millis() - t < 500) {
		driveSet(-127, -127);
		update();
		delay(10);
	}
} /* backUp */

void placeCone() {
	// Drop cone
	claw.power = 127;        // Open claw
	motorUpdate(&claw);
	delay(400);              // Give claw time to open
	claw.power = 0;          // Stop claw
	print("Cone placed!\n"); // Notify computer of cone state
	delay(325);              // Wait a lil bit
} /* placeCone */

Task placeConeT(void *none) {
	// Arm down
	armToPosition(ARM_DOWN + 35, 400);

	// Drop cone
	claw.power = 127;        // Open claw
	motorUpdate(&claw);
	delay(400);              // Give claw time to open
	claw.power = 0;          // Stop claw
	armToPosition(ARM_QUARTER, 400);
	print("Cone placed!\n"); // Notify computer of cone state
} /* placeCone */

void dropMogo(int zone) {
	int p[2] = { drive[0].sensor->value, drive[1].sensor->value };

	if (zone < 15) {
		// 10 point zone
		driveSet(30, 30);
		delay(500);
		mogoP(MOGO_DOWN - 100);
	} else {
		// 20 point zone
		driveSet(30, 30);
		delay(500);
		driveSet(127, 127);
		delay(250);
		waitForDriveStall(500);
		mogoP(MOGO_DOWN * .68);
	}
	driveToPosition(p[0] - 150, p[1] - 150, 2000);
} /* dropMogo */

void autonomous() {
	reset();
	sensorReset(drive[0].sensor);
	sensorReset(drive[1].sensor);
	sensorReset(arm.sensor);
	sensorReset(mogo.sensor);
	sensorReset(&gyro);

	selectedAuton = clipNum(selectedAuton, MAX_AUTON - 1, 0);
	autons[selectedAuton].execute();

	armSettings.target = arm.sensor->average; // Reset the arm position to it's
	                                          // current position

	while (isAutonomous()) {
		PID(&armSettings);                      // Hold the arm position
		update();
		delay(10);
	}
} /* autonomous */

void testMotors() {
	while (isAutonomous()) {
		for (int i = 1; i <= 10; i++) {
			motorSet(i, 127);
			delay(250);
			motorStopAll();
		}
	}
} /* testMotors */
