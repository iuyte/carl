/**
 * @file init.c
 * @brief Perform initialization and start handler tasks
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

void initializeIO() {}

void init() {
	// LCD initialization
	lcdInit(uart1);
	lcdSetBacklight(uart1, true);

	/**
	 * Notify both through the terminal and an lcd
	 *
	 * @param buffer the text to display
	 */
	void notice(const char *buffer) {
		print(buffer);
		lcdSetText(uart1, 2, buffer);
	} /* notice */

	print("\nInitializing... ");
	lcdSetText(uart1, 1, "Initializing...");

	// Set up the analog sensors
	mogoAngle = newAnalog(1);
	notice("mobile goal angle, ");

	gyro[0] = newGyro(2, 0);
	gyro[1] = newGyro(3, 0);
	notice("gyroscopes, ");

	// Set up the digital sensors
	armCoder = newQuad(1, 2, false);
	notice("arm quad, ");

	driveCoder[0] = newQuad(4, 5, true);
	notice("left drive quad, ");

	driveCoder[1] = newQuad(6, 7, false);
	notice("right drive quad, ");

	sonic = newSonic(3, 10);
	notice("ultrasonic sensor, ");

	// Initialize and set up all of the motors, servos, systems, etc
	claw = newMotor(5, false);
	notice("claw servo, ");

	arm[0] = newMotor(1, true);
	arm[1] = newMotor(10, false);
	notice("arm motors, ");

	mogo[0] = newMotor(4, false);
	mogo[1] = newMotor(7, true);
	notice("mobile goal motors, ");

	drive[0] = newMotor(2, true);
	notice("left drive motors, ");

	drive[1] = newMotor(9, false);
	notice("right drive motors, ");

	// Configure Systems
	Motor **drives[2] = {(Motor **)malloc(sizeof(Motor *)), (Motor **)malloc(sizeof(Motor *))};
	Motor **arms = (Motor **)malloc(sizeof(Motor *) * 2);
	Motor **mogos = (Motor **)malloc(sizeof(Motor *) * 2);

	drives[0][0] = drive[0];
	drives[1][0] = drive[1];
	arms[0] = arm[0];
	arms[1] = arm[1];
	mogos[0] = mogo[0];
	mogos[1] = mogo[1];

	confSystem(&Drive[0], driveCoder[0], 1, drives[0]);
	confSystem(&Drive[1], driveCoder[1], 1, drives[1]);
	confSystem(&Arm,      armCoder,      2, arms);
	confSystem(&Mogo,     mogoAngle,     2, mogos);
	notice("Systems, ");

	notice("done!");
	print("\n\n");
} /* init */
