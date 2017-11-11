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
 * ANY WARRANTY; without even the implied warranty ofMERCHANTABILITY or FITNESS
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

void   initLoop();

void   operatorControl() {
	initLoop();

	void moveDrive() {
		drive[0].power = joystickGetAnalog(1, 3);
		drive[1].power = joystickGetAnalog(1, 2);
	} /* drive */

	void moveMogo() {
		mogo[0].power = joystickGetDigital(1, 5, JOY_DOWN);
		mogo[1].power = joystickGetDigital(1, 5, JOY_UP);
	} /* moveMogo */

	void moveArm() {
		int power = -100 * digital(2, 6, JOY_UP, JOY_DOWN);

		if (armLimit.value) {
			armCoder.reset = true;
			power          = clipNum(power, 127, 0);
		}

		if (power) {
			arm[0].power = 127;
			arm[1].power = 127;
		} else {
			if ((armCoder.value < 500) && (armCoder.value >= 50)) {
				arm[0].power = -11;
				arm[1].power = -11;
			} else if (armCoder.value > 600) {
				arm[0].power = 11;
				arm[1].power = 11;
			} else {
				arm[0].power = 0;
				arm[1].power = 0;
			}
		}
	} /* moveArm */

	void moveClaw() {
		static bool lastClose = false;

		claw.power = joystickGetDigital(2, 5, JOY_DOWN) * 100 +
		             joystickGetDigital(2, 5, JOY_UP) * -100 +
		             lastClose * 15;

		if (joystickGetDigital(2, 5, JOY_UP)) {
			lastClose = true;
		} else if  (joystickGetDigital(2, 5, JOY_DOWN)) {
			lastClose = false;
		}
	} /* moveClaw */

	void moveLock() {
		int power =
		             joystickGetDigital(1, 6, JOY_UP) * 127 +
		             joystickGetDigital(1, 6, JOY_DOWN) * -127;

		mogo[0].power = power;
		mogo[1].power = power;
	} /* moveLock */

	void reset() {
		motorStopAll();
		arm[0].power = -50;
		arm[1].power = -50;
		delay(25);
		arm[0].power = 0;
		arm[1].power = 0;
		initLoop();
	} /* reset */

	while (true) {
		moveDrive();
		moveMogo();
		moveLock();
		moveArm();
		moveClaw();

		if (joystickGetDigital(1, 7, JOY_LEFT) &&
		    joystickGetDigital(2, 7, JOY_LEFT)) {
			reset();
		}

		  motorUpdate(&claw);

		for (size_t i = 0; i < 2; i++) {
			motorUpdate(&drive[i]);
			motorUpdate(&arm[i]);
			motorUpdate(&mogo[i]);
		}
		delay(20);
	}
} /* operatorControl */
