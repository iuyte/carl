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

void armToPosition(float pos) {
	armSettings.target = pos;

	do {
		PID(&armSettings);
		update();
		info();
		delay(10);
	} while (armSettings.error > 75 && armSettings.integral > 2.f);
} /* armToPosition */

void driveToPosition(int l, int r, int a, unsigned long until) {
	driveSettings[0].target = l;
	driveSettings[1].target = r;

	do {
		PID(&driveSettings[0]);
		PID(&driveSettings[1]);
		drive[0].power -= (a - gyro.average) * 1.5;
		drive[1].power += (a - gyro.average) * 1.5;

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
	}

	mogo->power = 0;
} /* mogoP */

void gyroP(int o, int precision) {
	int error      = 0;
	int derivative = 0;

	do {
		derivative = (o - gyro.average) - error;
		error      = o - gyro.average;

		drive[0].power = error * 2 + derivative * .2;
		drive[1].power = error * -2 + derivative * .2;

		update();
		info();
		delay(10);
	} while (abs(error) > precision ||
	         abs(drive[0].power) + abs(drive[1].power) > 20);

	drive[0].power = 0;
	drive[1].power = 0;
} /* gyroP */

int stage = 0;

void stageUp() {
	printf("\nStage %d", stage++);
} /* stageUp */

void autonomous() {
	reset();
	stageUp();

	unsigned long t = millis() + 250;
	claw.power = -50;
	while (millis() < t) {
		arm[0].power = -127;
		update();
	}
	arm[0].power = -27;
	stageUp();
	delay(1000);

	/*
	mogoP(2000);
	stageUp();
	delay(500);
	*/

	driveToPosition(2200, 2200, 2, millis() + 3000);
	stageUp();

	delay(750);

	armSettings.target = 100;
	t = millis() + 1500;
	do {
		arm[0].power = 127;
		update();
		info();
	} while (millis() < t);
	arm[0].power = 0;
	claw.power = 127;
	delay(500);
	claw.power = 50;

	t = millis() + 500;
	while (millis() < t) {
		arm[0].power = -127;
		update();
	}
	arm[0].power = 0;
	driveToPosition(500, 500, 0, millis() + 4000);

	/*
	mogoP(100);
	stageUp();
	*/



	// driveToPosition(1000, 1000, 0);
	gyroShutdown(gyro.pros);
	gyroShutdown(gyro.child->pros);
	ultrasonicShutdown(sonic.pros);
} /* autonomous */
