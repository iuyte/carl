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

void autonRightRed12()  {}

void autonLeftBlue12()  {}

void autonRightBlue12() {}

int   selectedAuton     = 0;
Auton autons[NUM_AUTON] = { {
															.name    = "red left 12",
															.execute = &autonLeftRed12,
														},{
															.name    = "red right 12",
															.execute = &autonRightRed12,
														},{
															.name    = "blue left 12",
															.execute = &autonLeftBlue12,
														},{
															.name    = "blue right 12",
															.execute = &autonRightBlue12,
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

void driveToPosition(int l, int r, int a, unsigned long until) {
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
	while (mogoAngle[1].value<p - 100 || mogoAngle[1].value>p + 100) {
		mogo.power = (p - mogoAngle[1].value) * -.22;
		update();
	}

	mogo.power = 0;
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
	          !gyroSettings[1].isTargetReached) || true);

	//						millis() < until);

	driveSet(0, 0);
} /* turnTo */

void getMogo() {
	claw.power         = -50;
	armSettings.target = 300;

	Task mogoUp(void *none) {
		while (isEnabled() && armCoder.value < 210) {
			delay(10);
		}
		mogoP(MOGO_DOWN);
	} /* mogoUp */

	GO(mogoUp, NULL);
	delay(750);

	int maxMin[2][2] = { { driveSettings[0].max, driveSettings[0].min },
											 { driveSettings[1].max, driveSettings[1].min } };
	int newPower = 80;
	driveSettings[0].max = newPower;
	driveSettings[1].max = newPower;
	driveSettings[0].min = -newPower;
	driveSettings[1].min = -newPower;
	driveToPosition(1200, 1200, 0, 1600);

	while (mogo.sensor->value < MOGO_DOWN - 150) {
		driveSet(40, 40);
		update();
	}

	driveToPosition(1450, 1450, 0, 900);
	driveSettings[0].max = maxMin[0][0];
	driveSettings[1].max = maxMin[1][0];
	driveSettings[0].min = maxMin[0][1];
	driveSettings[1].min = maxMin[1][1];

	delay(250);
	mogoP(MOGO_UP);
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
	sensorReset(&armCoder);
	sensorReset(&mogoAngle[0]);

	selectedAuton = clipNum(selectedAuton, NUM_AUTON - 1, 0);
	autons[selectedAuton].execute();
} /* autonomous */

Task placeCone(void *none) {
	claw.power = -50;
	armToPosition(25, 450);
	arm.power = 0;
	update();
	delay(175);
	claw.power = 127;
	update();
	delay(200);
	claw.power = 0;
	armToPosition(250, 500);
	arm.power = -25;
	mogoP(100);
	print("Cone placed!\n");
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
		driveToPosition(1500, 1800, 8, millis() + 2250);
	} else {
		driveToPosition(1500, 1800, 2, millis() + 2250);
	}


	mogoP(350);


	GO(backUp, NULL);

	TaskHandle coneHandle = GO(placeCone, NULL);

	if (selectedAuton) {
		driveToPosition(150, -150, 15, millis() + 2000);

		gyroPID(-162, 8);
	} else {
		driveToPosition(200, 400, 0, millis() + 2000);

		gyroPID(-160, 7);
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
	gyroPID(0, -expand(selectedAuton, .5, 10, -10));
	driveSet(-80, -80);

	delay(175);

	motorStopAll();

	if (coneHandle) {
		taskDelete(coneHandle);
	}
	printf("Ending autonomous with %lu to spare\n", t + 15000 - millis());
} /* autonomous */
