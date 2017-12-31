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
 * ANY WARRANTY; without even the implied warranty ofMERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License aint
 * with this program. If not, see <https://www.gnu.org/licenses/>
 */

#include "../include/auto.h"

void autonLeftRed12();

void autonRightRed12()  {}

void autonLeftBlue12()  {}

void autonRightBlue12() {}

int   selectedAuton     = 0;
Auton autons[NUM_AUTON] = {
	{
		.name    = "red left 12",
		.execute = &autonLeftRed12,
	},
	{
		.name    = "red right 12",
		.execute = &autonRightRed12,
	},
	{
		.name    = "blue left 12",
		.execute = &autonLeftBlue12,
	},
	{
		.name    = "blue right 12",
		.execute = &autonRightBlue12,
	},
};

void armToPosition(float pos, unsigned long until) {
	armSettings.target = pos;
	until             += millis();

	do {
		PID(&armSettings);
		update();
		delay(10);
	} while (!armSettings.isTargetReached && millis() < until);
} /* armToPosition */

void driveToPosition(int l, int r, int a, unsigned long until) {
	driveSettings[0].target = l;
	driveSettings[1].target = r;

	do {
		PID(&driveSettings[0]);
		PID(&driveSettings[1]);
		PID(&armSettings);
		drive[0].power += (a - gyro.average) * 1.5;
		drive[1].power -= (a - gyro.average) * 1.5;

		update();
		delay(10);
	} while (!driveSettings[0].isTargetReached ||
	         !driveSettings[1].isTargetReached);

	drive[0].power = 0;
	drive[1].power = 0;
	update();
} /* driveToPosition */

void mogoP(int p) {
	while (mogoAngle[1].value<p - 100 || mogoAngle[1].value>p + 100) {
		mogo.power = (p - mogoAngle[1].value) * -.22;
		update();
	}

	mogo.power = 0;
} /* mogoP */

float gyroPID[3] = { .278, .072, .307 };

void gyroP(int target, int precision) {
	int error      = 0;
	int integral   = 0;
	int derivative = 0;

	do {
		derivative = (target - gyro.average) - error;
		error      = target - gyro.average;
		integral  += error / 10;

		drive[0].power = -((error * gyroPID[0]) +
		                   (integral * gyroPID[1]) -
		                   (derivative * gyroPID[2]));
		drive[1].power = (error * gyroPID[0]) +
		                 (integral * gyroPID[1]) -
		                 (derivative * gyroPID[2]);

		update();
		delay(10);
	} while (abs(error) > precision ||
	         integral > 10);

	drive[0].power = 0;
	drive[1].power = 0;
} /* gyroP */

void autonomous() {
	reset();
	sensorReset(&armCoder);
	sensorReset(&mogoAngle[0]);

	selectedAuton = clipNum(selectedAuton, NUM_AUTON - 1, 0);
	autons[selectedAuton].execute();

	gyroShutdown(gyro.pros);
	gyroShutdown(gyro.child->pros);
	ultrasonicShutdown(sonic.pros);
} /* autonomous */

void getMogo() {
	Task driveForward(void *none) {
		driveToPosition(1600, 1600, 0, 2250);
	}
	GO(driveForward, NULL);

	claw.power = -50;
	armToPosition(-300, 300);
	mogoP(2150);
	delay(250);
	mogoP(350);
} /* getMogo */

Task backUp(void *time) {
	unsigned long t = (unsigned long)time;

	while (millis() - t < 14250) {
		delay(10);
	}

	while (isAutonomous()) {
		driveSet(-127, -127);
		delay(10);
		update();
	}
} /* backUp */

void auton() {
	print("\nBeginning of autonomous\n");
	unsigned long t = millis() - 5;

	/*
	 *   while (arm->sensor->value < 250 && millis() < t) {
	 *        arm[0].power = -127;
	 *        update();
	 *   }
	 */
	claw.power = -50;
	armToPosition(275, t + 500);
	arm.power = 50;
	delay(150);
	arm.power = -25;


	mogoP(2150);

	delay(250);

	print("\nAbout to start driving!");

	if (selectedAuton) {
		driveToPosition(1500, 1800, 8, millis() + 2250);
	} else {
		driveToPosition(1500, 1800, 2, millis() + 2250);
	}


	mogoP(350);


	GO(backUp, NULL);

	Task placeCone(void *none) {
		claw.power = -50;
		armToPosition(25, millis() + 450);
		arm.power = 0;
		update();
		delay(175);
		claw.power = 127;
		update();
		delay(200);
		claw.power = 0;
		armToPosition(250, millis() + 500);
		arm.power = -25;
		mogoP(100);
		print("Cone placed!\n");
	} /* placeCone */

	TaskHandle coneHandle = GO(placeCone, NULL);

	if (selectedAuton) {
		driveToPosition(150, -150, 15, millis() + 2000);

		gyroP(-162, 8);
	} else {
		driveToPosition(200, 400, 0, millis() + 2000);

		gyroP(-160, 7);
	}

	reset();


	if (selectedAuton) {
		driveToPosition(675, 750, -8, millis() + 1600);
		driveSet(70, 40);
	} else {
		driveToPosition(250,  250,  7,  millis() + 1400);
		driveToPosition(2550, 2250, 30, millis() + 1400);
	}
	update();
	reset();


	mogoP(2000);


	mogo.power = -50;
	update();
	delay(250);

	driveSet(-127, -127);
	update();


	delay(250);
	mogo.power = 50;
	update();
	delay(350);
	driveSet(-100, -100);
	mogo.power = 0;
	arm.power  = 80;
	update();

	delay(400);

	arm.power = 0;
	gyroP(0, -expand(selectedAuton, .5, 10, -10));
	driveSet(-80, -80);

	delay(175);

	motorStopAll();

	if (coneHandle) {
		taskDelete(coneHandle);
	}
	printf("Ending autonomous with %lu to spare\n", t + 15000 - millis());
} /* autonomous */
