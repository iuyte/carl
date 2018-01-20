/**
 * @file autoRightRed.c
 * @brief Right side red alliance autonomous routines
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

void autonRightRed12() {
	getMogo();                       // Get the mobile goal

	turnTo(5, 425);                  // Align to a right tilt
	driveSettings[0].max -= 40;      // Limit left side speed
	driveToPosition(800, 400, 1550); // Back up
	driveSettings[0].max += 40;      // Correct speed
	turnTo(165, 2000);               // Turn around
	placeCone();                     // Place the cone

	// Reset drive encoders & gyro
	sensorReset(&driveCoder[0]);
	sensorReset(&driveCoder[1]);
	sensorReset(&gyro);

	driveToPositionAngle(840, 900, -6, 1350); // Drive arc -13 degrees
	mogoP(MOGO_DOWN);

	driveSet(-127, -127);                     // Back up the drive
	delay(130);
	mogoP(MOGO_DOWN - 300);                   // Bring the mobile goal up a bit
	delay(250);
	driveSet(0, 0);                           // Stop the drive

	armSettings.target = armCoder.average;    // Reset the arm position to
	                                          // it's current position
} /* autonRightRed12 */
