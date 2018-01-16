/**
 * @file autoLeftRed.c
 * @brief Left side red alliance autonomous routines
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

void autonLeftRed12() {
	getMogo();                       // Get the mobile goal
	placeCone();                     // Place the cone

	turnTo(-5, 300);                 // Align to a left tilt
	driveSettings[1].max -= 40;      // Limit right side speed
	driveToPosition(400, 800, 1550); // Back up
	driveSettings[1].max += 40;      // Correct speed
	turnTo(-165, 2000);              // Turn around

	// Reset drive encoders & gyro
	sensorReset(&driveCoder[0]);
	sensorReset(&driveCoder[1]);
	sensorReset(&gyro);

	driveToPositionAngle(800, 700, 13, 1200); // Drive arc 13 degrees clockwise
	mogoP(MOGO_DOWN);

	driveSet(-127, -127);                     // Back up the drive
	delay(130);
	mogoP(MOGO_DOWN - 300);                   // Bring the mobile goal up a bit
	delay(250);
	driveSet(0, 0);                           // Stop the drive

	armSettings.target = armCoder.average;    // Reset the arm position to it's
	                                          // current position
} /* autonLeftRed12 */

void autonLeftRed22() {
	getMogo();                       // Get the mobile goal
	placeCone();                     // Place the cone

	turnTo(-5, 300);                 // Align to a left tilt
	driveSettings[1].max -= 40;      // Limit right side speed
	driveToPosition(200, 600, 1550); // Back up
	driveSettings[1].max += 40;      // Correct speed
	turnTo(-175, 2000);              // Turn around

	// Reset drive encoders & gyro
	sensorReset(&driveCoder[0]);
	sensorReset(&driveCoder[1]);
	sensorReset(&gyro);

	driveToPositionAngle(900, 1000, -5, 1200); // Drive arc -5 degrees clockwise
	mogoP(MOGO_DOWN);

	driveSet(-127, -127);                      // Back up the drive
	delay(130);
	mogoP(MOGO_DOWN - 300);                    // Bring the mobile goal up a bit
	delay(250);
	driveSet(0, 0);                            // Stop the drive

	armSettings.target = armCoder.average;     // Reset the arm position to it's
	                                           // current position
} /* autonLeftRed22 */
