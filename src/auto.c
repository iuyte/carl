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
void autonTest();

int   selectedAuton     = 4;
Auton autons[NUM_AUTON] = { {
															.name    = "red left 12",
															.execute = &autonLeftRed12,
														},{
															.name    = "red right 12",
															.execute = &autonRightRed12,
														},{
															.name    = "blue left 12",
															.execute = &autonRightRed12,
														},{
															.name    = "blue right 12",
															.execute = &autonLeftRed12,
														},{
															.name    = "red left 22",
															.execute = &autonLeftRed22,
														},{
															.name    = "test",
															.execute = &autonTest,
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
	while (isAutonomous() && abs(p - mogoAngle.average) > 75) {
		mogo.power = sgn(p - mogoAngle.average) * 127;
		PID(&armSettings);
		update();
	}

	mogo.power = 0;
	motorUpdate(&mogo);
} /* mogoP */

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
	armSettings.target = ARM_QUARTER - 100;

	Task mogoDown(void *none) {
		mogoP(MOGO_DOWN);
	} /* mogoDown */

	Task mogoUp(void *none) {
		mogoP(MOGO_UP);
		taskDelete(NULL);
	} /* mogoUp */

	GO(mogoDown, NULL);
	delay(750);

	driveToPosition(2150, 2150, 2000);

	TaskHandle mogoUpHandle = GO(mogoUp, NULL);
	driveToPosition(2400, 2400, 800);

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

void autonomous() {
	reset();
	sensorReset(&driveCoder[0]);
	sensorReset(&driveCoder[1]);
	sensorReset(&armCoder);
	sensorReset(&mogoAngle);
	sensorReset(&gyro);

	selectedAuton = clipNum(selectedAuton, NUM_AUTON - 1, 0);
	autons[selectedAuton].execute();

	armSettings.target = armCoder.average; // Reset the arm position to it's
	                                       // current position

	while (isAutonomous()) {
		PID(&armSettings);                   // Hold the arm position
		update();
		delay(10);
	}
} /* autonomous */

void placeCone() {
	// Drop cone
	claw.power = 127;        // Open claw
	motorUpdate(&claw);
	delay(400);              // Give claw time to open
	claw.power = 0;          // Stop claw
	print("Cone placed!\n"); // Notify computer of cone state
} /* placeCone */

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
		driveToPositionAngle(1500, 1800, 8, millis() + 2250);
	} else {
		driveToPositionAngle(1500, 1800, 2, millis() + 2250);
	}


	mogoP(350);

	GO(backUp, NULL);

	TaskHandle coneHandle = GO(placeCone, NULL);

	if (selectedAuton) {
		driveToPositionAngle(150, -150, 15, millis() + 2000);

		gyroPID(-162, 8);
	} else {
		driveToPositionAngle(200, 400, 0, millis() + 2000);

		gyroPID(-160, 7);
	}

	reset();


	if (selectedAuton) {
		driveToPositionAngle(675, 750, -8, millis() + 1600);
		driveSet(70, 40);
	} else {
		driveToPositionAngle(250,  250,  7,  millis() + 1400);
		driveToPositionAngle(2550, 2250, 30, millis() + 1400);
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
	gyroPID(0, -expand(selectedAuton, .5, 10, -10));
	driveSet(-80, -80);

	delay(175);

	motorStopAll();

	if (coneHandle) {
		taskDelete(coneHandle);
	}
	printf("Ending autonomous with %lu to spare\n", t + 15000 - millis());
} /* autonomous */
