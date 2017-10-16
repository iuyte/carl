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

void   operatorControl() {
	void moveDrive() {
		drive[0]->power = deadBand(joystickGetAnalog(1, 3), 10);
		drive[1]->power = deadBand(joystickGetAnalog(1, 2), 10);
	} /* drive */

	void moveMogo() {
		mogo->power = joystickGetDigital(1, 6, JOY_DOWN) * -127 +
		                 joystickGetDigital(1, 6, JOY_UP) * 127;
	} /* moveMogo */

	void moveArm() {
		arm->power = joystickGetDigital(2, 6, JOY_DOWN) * -127 +
		                 joystickGetDigital(2, 6, JOY_UP) * 127;
	} /* moveArm */

	void moveClaw() {
		claw->position = joystickGetDigital(2, 5, JOY_DOWN) * -127 +
		                 joystickGetDigital(2, 6, JOY_UP)   *  127;
	} /* moveClaw */

	void info() {
		printf("\r| %5d     | %5d     | %5d     | ",
		       drive[0]->power,
		       drive[1]->power,
		       arm->power);
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
