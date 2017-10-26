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

void operatorControl() {
	gyro[0]->reset = true;
	gyro[1]->reset = true;

	void moveDrive() {
		drive[0]->power = joystickGetAnalog(1, 3);
		drive[1]->power = joystickGetAnalog(1, 2);
	} /* drive */

	void moveMogo() {
		mogo[0]->power = joystickGetDigital(1, 5, JOY_DOWN) * -127 +
		                 joystickGetDigital(1, 5, JOY_UP) * 127;
		mogo[1]->power = joystickGetDigital(1, 5, JOY_DOWN) * -127 +
		                 joystickGetDigital(1, 5, JOY_UP) * 127;
	} /* moveMogo */

	void moveArm() {
		arm[0]->power = joystickGetDigital(1, 6, JOY_DOWN) * -127 +
		                 joystickGetDigital(1, 6, JOY_UP) * 127;
		arm[1]->power = joystickGetDigital(1, 6, JOY_DOWN) * -127 +
		                 joystickGetDigital(1, 6, JOY_UP) * 127;
	} /* moveArm */

	void moveClaw() {
		// claw->power = clipNum(claw->power + (joystickGetDigital(1, 7, JOY_DOWN) ?
		// -3 : (joystickGetDigital(1, 7, JOY_UP) ? 2 : 0)), 100, 0);
		claw->power = joystickGetDigital(1, 7, JOY_DOWN) * -100 +
		                 joystickGetDigital(1, 7, JOY_UP) * 100;
	} /* moveClaw */

	void info() {
		printf(
		  "\r| %5ld     | %5ld     | %5ld     | %5ld     | %5d     | %5ld     | %5ld     | ",
		  driveCoder[0]->value,
		  driveCoder[1]->value,
		  armCoder->value,
		  mogoAngle->value,
		  claw->power,
		  gyros(),
		  sonic->value);
	} /* info */

	while (isEnabled()) {
		moveDrive();
		moveMogo();
		moveArm();
		moveClaw();
		info();
		delay(20);
	}
} /* operatorControl */
