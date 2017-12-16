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

#include "../include/robot.h"

void armToPosition(float pos, unsigned long until) {
	armSettings.target = pos;
	int error = 100;

	do {
		// PID(&armSettings);
		error      = pos - arm->sensor->value;
		arm->power = -error;
		update();
		info();
		delay(10);
	} while (abs(error) > 15 && millis() < until && !armLimit[1].value);
} /* armToPosition */

void driveToPosition(int l, int r, int a, unsigned long until) {
	driveSettings[0].target = l;
	driveSettings[1].target = r;

	do {
		PID(&driveSettings[0]);
		PID(&driveSettings[1]);
		drive[0].power += (a - gyro.average) * 1.5;
		drive[1].power -= (a - gyro.average) * 1.5;

		update();
		info();
		delay(10);
	} while ((driveSettings[0].integral > 3.f ||
	          driveSettings[1].integral > 3.f ||
	          driveSettings[0].error > 100 ||
	          driveSettings[1].error > 100) &&
	         millis() < until);

	drive[0].power = 0;
	drive[1].power = 0;
} /* driveToPosition */

void mogoP(int p) {
	while (mogoAngle[1].value<p - 100 || mogoAngle[1].value>p + 100) {
		mogo->power = (p - mogoAngle[1].value) * -.22;
		update();
		info();
	}

	mogo->power = 0;
} /* mogoP */

float gyroPID[3] = { .267, .071, .31 };

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
		info();
		delay(10);
	} while (abs(error) > precision ||
	         integral > 10);

	drive[0].power = 0;
	drive[1].power = 0;
} /* gyroP */

int stage = 0;

void stageUp() {
	printf("Stage %d\n", stage++);
} /* stageUp */

void auton();

void autonomous() {
	reset();
	sensorReset(&armCoder);
	sensorReset(&mogoAngle[0]);
	auton();
	gyroShutdown(gyro.pros);
	gyroShutdown(gyro.child->pros);
	ultrasonicShutdown(sonic.pros);
} /* autonomous */

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
	armToPosition(250, t + 500);
	arm[0].power = 50;
	delay(150);
	arm[0].power = -25;
	stageUp();

	mogoP(2000);
	stageUp();
	delay(250);

	print("\nAbout to start driving!");
	if (selectedAuton) {
		driveToPosition(1500, 1800, 8, millis() + 2250);
	} else {
		driveToPosition(1500, 1800, 2, millis() + 2250);
	}
	stageUp();

	mogoP(350);
	stageUp();

	Task placeCone(void *none) {
		claw.power = -50;
		armToPosition(25, millis() + 450);
		arm[0].power = 0;
		update();
		delay(175);
		claw.power = 127;
		update();
		delay(200);
		claw.power = 0;
		armToPosition(250, millis() + 500);
		arm[0].power = -25;
		mogoP(100);
		print("Cone placed!\n");
	} /* placeCone */

	GO(placeCone, NULL);

	if (selectedAuton) {
		driveToPosition(300, 100, 13, millis() + 2000);
		stageUp();
		gyroP(-160, 8);
	} else {
		driveToPosition(200, 400, 0, millis() + 2000);
		stageUp();
		gyroP(-160, 7);
	}

	reset();
	stageUp();

	if (selectedAuton) {
		driveToPosition(700, 675, 6, millis() + 2000);
		driveSet(50, 50);
	} else {
		driveToPosition(250, 250, 7, millis() + 1400);
		driveToPosition(2550, 2250, 30, millis() + 1400);
	}
	update();
	reset();
	stageUp();

	mogoP(2000);
	stageUp();

	mogo[0].power = -50;
	update();
	delay(250);

	driveSet(-127, -127);
	update();
	stageUp();

	delay(250);
	mogo[0].power = 50;
	update();
	delay(350);
	mogo[0].power = 0;
	arm[0].power = 80;
	update();
	stageUp();
	delay(400);
	
	arm[0].power = 0;
	gyroP(0, -expand(selectedAuton, .5, 10, -10));
	driveSet(-127, -127);
	stageUp();

	motorStopAll();
	printf("Ending autonomous with %lu to spare\n", t + 15000 - millis());
} /* autonomous */
