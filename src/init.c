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
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <https://www.gnu.org/licenses/>
 */

#include "../include/robot.h"

float recalc(int p);

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
	gyro        = newGyro(1, true, 199);
	gyro.child  = (Sensor *)malloc(sizeof(Sensor));
	*gyro.child = newGyro(2, true, 198);
	notice("gyroscopes, ");
	mogoAngle        = newAnalog(8, true);
	mogoAngle.child  = (Sensor *)(malloc(sizeof(Sensor)));
	*mogoAngle.child = newAnalog(7, true);
	notice("mobile goal angle, ");
	clawAngle          = newAnalog(5, true);
	clawAngle.inverted = true;
	notice("claw angle, ");

	// Set up the digital sensors
	armCoder = newQuad(1, 2, false);
	notice("arm quad, ");
	sonic = newSonic(3, 10);
	ultrasonicShutdown(sonic.pros);
	notice("ultrasonic sensor, ");
	driveCoder[0] = newQuad(4, 5, true);
	driveCoder[0].recalc = &recalc;
	notice("left drive quad, ");
	driveCoder[1] = newQuad(8, 9, true);
	notice("right drive quad, ");
	armLimit[0] = newDigital(12, true);
	armLimit[1] = newDigital(11, true);
	notice("arm limit switch, ");

	// Initialize and set up all of the motors, servos, etc
	claw        = motorCreate(3, false);
	claw.sensor = &clawAngle;
	notice("claw motor, ");

	arm        = motorCreate(5,  false);
	arm.child  = (Motor *)(malloc(sizeof(Motor)));
	*arm.child = motorCreate(6, true);
	arm.sensor = &armCoder;
	notice("arm motors, ");

	mogo        = motorCreate(1, false);
	mogo.child  = (Motor *)(malloc(sizeof(Motor)));
	*mogo.child = motorCreate(10, true);
	notice("mobile goal motors, ");

	drive[0]        = motorCreate(2, true);
	drive[0].child  = (Motor *)(malloc(sizeof(Motor)));
	*drive[0].child = motorCreate(4, true);
	drive[0].sensor = &driveCoder[0];

	drive[1]        = motorCreate(9, false);
	drive[1].child  = (Motor *)(malloc(sizeof(Motor)));
	*drive[1].child = motorCreate(7, false);
	drive[1].sensor = &driveCoder[1];
	notice("drive motors, ");

	notice("done!");
	lcdSetText(uart1, 1, "Ready!");
	print("\n\n");
	info();
	setTeamName("709S");

	// Start the LCD task
	LCDHandle = GO(lcdTask, NULL);
} /* init */

float recalc(int p) {
	return p * 8 / 5;
}
