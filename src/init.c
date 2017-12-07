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
 * You should have received a copy of the GNU General Public License aint
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

	// Set up the analog sensors // *
	// gyro = newGyro(1, false, 198);
	// notice("gyroscopes, ");
	// mogoAngle[0]       = newAnalog(3, false);
	// mogoAngle[1]       = newAnalog(4, false);
	// mogoAngle[0].child = &mogoAngle[1];
	// notice("mobile goal angle, ");
	// clawAngle = newAnalog(5, true);
	// notice("claw angle, ");

	// Set up the digital sensors
	// armCoder = newQuad(1, 2, false);
	// notice("arm quad, ");
	// driveCoder[0] = newQuad(4, 5, false);
	// notice("left drive quad, ");
	// driveCoder[1] = newQuad(6, 7, true);
	// notice("right drive quad, ");
	// sonic = newSonic(3, 10);
	// notice("ultrasonic sensor, ");
	// armLimit = newDigital(11, true);
	// notice("arm limit switch, ");

	// */

	// Initialize and set up all of the motors, servos, etc
	claw = motorCreate(5, false);
	notice("claw servo, ");

	arm[0]     = motorCreate(1,  false);
	arm[1]     = motorCreate(10, true);
	arm->child = &arm[1];
	notice("arm motors, ");

	mogo[0]     = motorCreate(4, false);
	mogo[1]     = motorCreate(7, true);
	mogo->child = &mogo[1];
	notice("mobile goal motors, ");

	drive[0]     = motorCreate(2, true);
	drive[1]     = motorCreate(9, false);
	drive->child = &drive[1];
	notice("drive motors, ");

	notice("done!");
	print("\n\n");
	lcdSetText(uart1, 1, "Battery:");
	setTeamName("709S");
} /* init */
