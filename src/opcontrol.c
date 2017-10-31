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

Settings armSettings;

int digital(unsigned char joyNum,
            unsigned char channel,
            unsigned char b1,
            unsigned char b2) {
	return joystickGetDigital(joyNum, channel, b2) * -1 +
	       joystickGetDigital(joyNum, channel, b1) * 1;
} /* digital */

void operatorControl() {
	armCoder->reset = true;
	armSettings     = newSettings(0,
	                              .5,
	                              0,
	                              0,
	                              &Arm,
	                              false,
	                              127,
	                              -127,
	                              2,
	                              8,
	                              5);
	GO(PID, &armSettings);
	armSettings.isActive = false;

	sensorTake(gyro[0], -1);
	sensorTake(gyro[1], -1);
	gyro[0]->reset = true;
	gyro[1]->reset = true;
	sensorGive(gyro[0]);
	sensorGive(gyro[1]);

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
		static int power = 0;

		// armSettings.target += .5 * digital(1, 6, JOY_UP, JOY_DOWN);
		power = 127 * digital(1, 6, JOY_UP, JOY_DOWN);

		if (power) {
			armSettings.isActive = false;
			armSettings.target   = armCoder->value;
			Arm.power            = power;
			arm[0]->power        = power;
			arm[1]->power        = power;
		} else {
			armSettings.isActive = true;
		}

		/*
		 *   if (digital(1, 6, JOY_UP, JOY_DOWN) == power) {
		 *        armSettings.isActive = true;
		 *   } else {
		 *        armSettings.isActive = false;
		 *        power                = 127 * digital(1, 6, JOY_UP, JOY_DOWN);
		 *        armSettings.target   = armSettings.system->sensor->value;
		 *        Arm.power            = power;
		 *   }
		 */
	} /* moveArm */

	void moveClaw() {
		// claw->power = clipNum(claw->power + (joystickGetDigital(1, 7, JOY_DOWN) ?
		// -3 : (joystickGetDigital(1, 7, JOY_UP) ? 2 : 0)), 100, 0);
		claw->power = joystickGetDigital(1, 8, JOY_DOWN) * 50;
	} /* moveClaw */

	while (true) {
		moveDrive();
		moveMogo();
		moveArm();
		moveClaw();
		delay(20);
	}
} /* operatorControl */
