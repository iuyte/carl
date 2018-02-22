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

void autonRehuh();

int   selectedAuton         = 8;
Auton autons[MAX_AUTON + 1] =
{ {
		// index 0
		.name       = "< red left 12 >",
		.sensorName = "arm",
		.sensor     = &arm.sensor,
		.execute    = &autonLeftRed12,
	},{
		// index 1
		.name       = "< red left 22  >",
		.sensorName = "gyr2",
		.sensor     = &gyro.child,
		.execute    = &autonLeftRed22,
	},{
		// index 2
		.name       = "< red right 12 >",
		.sensorName = "claw",
		.sensor     = &claw.sensor,
		.execute    = &autonRightRed12,
	},{
		// index 3
		.name       = "< red right 22 >",
		.sensorName = "mgo",
		.sensor     = &mogo.sensor,
		.execute    = &autonRightRed22,
	},{
		// index 4
		.name       = "< blue left 12 >",
		.sensorName = "mgo",
		.sensor     = &mogo.sensor,
		.execute    = &autonLeftRed12,
	},{
		// index 5
		.name       = "< blue left 22  >",
		.sensorName = "mgo",
		.sensor     = &mogo.sensor,
		.execute    = &autonLeftRed22,
	},{
		// index 6
		.name       = "< blue right 12>",
		.sensorName = "mgo",
		.sensor     = &mogo.sensor,
		.execute    = &autonRightRed12,
	},{
		// index 7
		.name       = "< blue right 22 >",
		.sensorName = "mgo",
		.sensor     = &mogo.sensor,
		.execute    = &autonRightRed22,
	},{
		// index 8
		.name       = "<    skills    >",
		.sensorName = "mgo",
		.sensor     = &mogo.sensor,
		.execute    = &autonSkills,
	},{
		// index 9
		.name       = "<     none     >",
		.sensorName = "mgo",
		.sensor     = &arm.sensor,
		.execute    = &autonNone,
	},{
		// index 10
		.name       = "<     test     >",
		.sensorName = "lef",
		.sensor     = &drive[0].sensor,
		.execute    = &autonTest,
	},{
		// index 11
		.name       = "< test motors  >",
		.sensorName = "rit",
		.sensor     = &drive[1].sensor,
		.execute    = &testMotors,
	},{
		// index 12
		.name       = "<    rehuh?    >",
		.sensorName = "rit",
		.sensor     = &drive[1].sensor,
		.execute    = &autonRehuh,
	}, };

void armToPosition(float pos, unsigned long until) {
	armSettings.target = pos;
	until             += millis();

	do {
		PID(&armSettings);
		motorUpdate(&arm);
		sensorRefresh(arm.sensor);
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

		motorUpdate(&arm);
		sensorRefresh(arm.sensor);
		sensorRefresh(&gyro);

		for (int i = 0; i < 2; i++) {
			motorUpdate(&drive[i]);
			sensorRefresh(drive[i].sensor);
		}

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

		motorUpdate(&arm);
		sensorRefresh(arm.sensor);
		sensorRefresh(&gyro);

		for (int i = 0; i < 2; i++) {
			motorUpdate(&drive[i]);
			sensorRefresh(drive[i].sensor);
		}

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
	delay(300);

	GO(mogoPT, MOGO_DOWN + 10);
	delay(750);

	driveToPosition(2350, 2350, 2300);

	TaskHandle mogoUpHandle = GO(mogoPT, MOGO_UP);
	driveToPosition(2600, 2600, 525);

	while (taskGetState(mogoUpHandle)) {
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
	// Arm down
	armToPosition(ARM_DOWN + 35, 400);

	// Drop cone
	claw.power = 127; // Open claw
	motorUpdate(&claw);
	delay(400);       // Give claw time to open
	claw.power = 0;   // Stop claw
	armToPosition(ARM_QUARTER, 400);
	arm.power = 10;
	motorUpdate(&arm);
	print("Cone placed!\n"); // Notify computer of cone state
	delay(150);
} /* placeCone */

Task placeConeT(void *none) {
	placeCone();
} /* placeCone */

TaskHandle dropMogo20(TaskHandle mogoHandle) {
	int p[2] = { drive[0].sensor->value, drive[1].sensor->value };

	// Start the mogo intake down
	if (!mogoHandle && taskGetState(mogoHandle))
		mogoHandle = GO(mogoPT, MOGO_MID + 125);
	driveToPosition(p[0] + 250, p[1] + 250, 600);
	driveSet(30, 30);

	// Wait until the mogo intake is up
	while (taskGetState(mogoHandle)) {
		delay(10);
	}

	// Wait a bit for the mobile goal to settle
	mogo.power = 64;
	motorUpdate(&mogo);
	driveSet(64, 64);
	delay(200);
	mogo.power = 30;

	driveToPosition(p[0] - 400, p[1] - 400, 3000);
	return GO(mogoPT, MOGO_UP);
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

Task driveToPositionAngleT(void *triple) {
	Triple *t = (Triple *)triple;

	driveSettings[0].target = t->a;
	driveSettings[1].target = t->b;
	unsigned long until = millis() + (t->a + t->b) * 2;

	do {
		PID(&driveSettings[0]);
		PID(&driveSettings[1]);
		drive[0].power += (t->c - gyro.average) * 2.3;
		drive[1].power -= (t->c - gyro.average) * 2.3;

		motorUpdate(&arm);
		sensorRefresh(arm.sensor);
		sensorRefresh(&gyro);

		for (int i = 0; i < 2; i++) {
			motorUpdate(&drive[i]);
			sensorRefresh(drive[i].sensor);
		}

		delay(10);
	} while ((!driveSettings[0].isTargetReached ||
	          !driveSettings[1].isTargetReached));

	drive[0].power = 0;
	drive[1].power = 0;
	delete(triple);
	taskDelete(NULL);
} /* driveToPositionAngleT */

Task armToPositionT(void *pos) {
	int p = (int)pos;

	armToPosition(p, p * 1.34);
	taskDelete(NULL);
} /* armToPositionT */

Task clawToPositionT(void *pos) {
	clawSettings.target = (int)pos;

	do {
		PID(&clawSettings);
		motorUpdate(&claw);
		sensorRefresh(claw.sensor);
		delay(10);
	} while (!clawSettings.isTargetReached);

	taskDelete(NULL);
} /* clawToPositionT */

Task die(void *none) {
	taskDelete(NULL);
}

void moveTo(int leftV, int rightV, int armV, int mogoV, int clawV, int gyroV) {
	TaskHandle driveHandle, armHandle, mogoHandle, clawHandle = NULL;
	update();

	if ((abs(leftV - drive[0].sensor->value) > driveSettings[0].tolerance) ||
	    (abs(rightV - drive[1].sensor->value) > driveSettings[1].tolerance) || 
			(abs(gyroV - gyro.average) > gyroSettings[0].tolerance)) {
		Triple *t = new(Triple);
		t->a        = leftV;
		t->b        = rightV;
		t->c        = gyroV;
		driveHandle = GO(driveToPositionAngleT, t);
	}

	if (abs(armV - arm.sensor->value) > armSettings.tolerance)
		armHandle = GO(armToPositionT, armV);

	if (abs(mogoV - mogo.sensor->average) > 90)
		mogoHandle = GO(mogoPT, mogoV);

	if (abs(clawV - claw.sensor->value) > clawSettings.tolerance)
		clawHandle = GO(clawToPositionT, clawV);

	if (driveHandle)
		while (taskGetState(driveHandle))
			delay(10);

	if (armHandle)
		while (taskGetState(armHandle))
			delay(10);

	if (mogoHandle)
		while (taskGetState(mogoHandle))
			delay(10);

	if (clawHandle)
		while (taskGetState(clawHandle))
			delay(10);
} /* moveTo */
