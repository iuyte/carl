/**
 * @file opcontrol.c
 * @brief Controls what happens in operator control
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

int digital(unsigned char joyNum,
            unsigned char channel,
            unsigned char b1,
            unsigned char b2) {
	return joystickGetDigital(joyNum, channel, b2) * -1 +
	       joystickGetDigital(joyNum, channel, b1) * 1;
} /* digital */

void operatorControl() {
	printf("Starting Driver Control...\n");
	ultrasonicShutdown(sonic.pros);
	reset();
	void moveDrive() {
		drive[0].power = deadBand(joystickGetAnalog(1, 3), 10);
		drive[1].power = deadBand(joystickGetAnalog(1, 2), 10);
	} /* drive */

	void moveMogo() {
		mogo.power = joystickGetDigital(1, 5, JOY_DOWN) * -127 +
		             joystickGetDigital(1, 5, JOY_UP) * 127;
	} /* moveMogo */

	void moveArm() {
		static unsigned long lastPress;

		if (digital(2, 6, JOY_UP, JOY_DOWN) || (millis() - lastPress < 175)) {
			arm.power = -127 * digital(2, 6, JOY_UP, JOY_DOWN);

			if (arm.power) {
				lastPress = millis();
			}

			if (armLimit[0].value) {
				sensorReset(&armCoder);
				arm.power = clipNum(arm.power, 127, 0);
			} else if (armLimit[1].value) {
				arm.power = clipNum(arm.power, 0, -127);
			}
			armSettings.target = armCoder.value;
		} else if (armLimit[0].value || armLimit[1].value) {
			arm.power = 0;
		} else {
			PID(&armSettings);
		}
	} /* moveArm */

	void moveClaw() {
		static bool lastClose = false;

		claw.power = joystickGetDigital(2, 5, JOY_DOWN) * -100 +
		             joystickGetDigital(2, 5, JOY_UP) * 100 +
		             lastClose * 5;

		if (joystickGetAnalog(2, 4)) {
			claw.power = .85 * joystickGetAnalog(2, 4);
		}

		if (joystickGetDigital(2, 5, JOY_UP)) {
			lastClose = true;
		} else if  (joystickGetDigital(2, 5, JOY_DOWN)) {
			lastClose = false;
		}
	} /* moveClaw */

	void moveLock() {
		int power = joystickGetDigital(1, 6, JOY_UP) * 127 +
		             joystickGetDigital(1, 6, JOY_DOWN) * -127;

		mogo.power = power;
	} /* moveLock */

	printf("Beginning driver control loop\n");

	while (true) {
		if (joystickGetDigital(1, 7, JOY_LEFT) &&
		    joystickGetDigital(2, 7, JOY_LEFT)) {
			exit(0);
		}

		moveDrive();
		moveMogo();
		moveLock();
		moveArm();
		moveClaw();

		update();
		info();
		delay(20);
	}
} /* operatorControl */
