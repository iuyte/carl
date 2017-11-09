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
	gyroConf(&gyro, 1, false, 198);
	notice("gyroscopes, ");

	analogConf(&mogoAngle,           3, false);
	analogConf(mogoAngle.redundancy, 4, false);
	notice("mobile goal angle, ");

	analogConf(&clawAngle, 5, true);
	notice("claw angle, ");

	// Set up the digital sensors
	quadConf(&armCoder, 1, 2, false);
	notice("arm quad, ");

	quadConf(&driveCoder[0], 4, 5, false);
	notice("left drive quad, ");

	quadConf(&driveCoder[0], 6, 7, true);
	notice("right drive quad, ");

	sonicConf(&sonic, 3, 10);
	notice("ultrasonic sensor, ");

	digitalConf(&armLimit, 11, true);
	notice("arm limit switch, ");

	// Initialize and set up all of the motors, servos, systems, etc
	motorConf(&claw, 5, false);
	notice("claw servo, ");

	motorConf(&arm[0], 1,  false);
	motorConf(&arm[1], 10, true);
	notice("arm motors, ");

	motorConf(&mogo[0], 4, false);
	motorConf(&mogo[1], 7, true);
	notice("mobile goal motors, ");

	// drive[0] = newMotor(2, true);
	notice("left drive motors, ");

	// drive[1] = newMotor(9, false);
	notice("right drive motors, ");

	// Configure Systems
	Motor **drives[2] = {
		(Motor **)malloc(sizeof(Motor *)),
		(Motor **)malloc(sizeof(Motor *))
	};

	Motor **arms  = (Motor **)malloc(sizeof(Motor *) * 2);
	Motor **mogos = (Motor **)malloc(sizeof(Motor *) * 2);

	drives[0][0] = &drive[0];
	drives[1][0] = &drive[1];
	arms[0]      = &arm[0];
	arms[1]      = &arm[1];
	mogos[0]     = &mogo[0];
	mogos[1]     = &mogo[1];

	confSystem(&Drive[0], &driveCoder[0], 1, drives[0]);
	confSystem(&Drive[1], &driveCoder[1], 1, drives[1]);
	confSystem(&Arm,      &armCoder,      2, arms);
	confSystem(&Mogo,     &mogoAngle,     2, mogos);
	notice("Systems, ");

	notice("done!");
	print("\n\n");
	lcdSetText(uart1, 1, "Battery:");
} /* init */

void initLoop() {
	gyro.reset                  = true;
	mogoAngle.reset             = true;
	mogoAngle.redundancy->reset = true;
	driveCoder[0].reset         = true;
	driveCoder[1].reset         = true;
	armCoder.reset              = true;
	clawAngle.reset             = true;
} /* initLoop */
