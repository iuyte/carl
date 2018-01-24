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

void autonLeftBlue12() {
	getMogo();                       // Get the mobile goal

	turnTo(2, 300);                  // Align to be almost straight
	driveSettings[1].max -= 40;      // Limit right side speed
	driveToPosition(350, 750, 2150); // Back up
	driveSettings[1].max += 40;      // Correct speed
	turnTo(-165, 2100);              // Turn around
	placeCone();                     // Place the cone

	// Reset drive encoders & gyro
	sensorReset(drive[0].sensor);
	sensorReset(drive[1].sensor);
	sensorReset(&gyro);

	driveToPositionAngle(770, 880, 13, 1600); // Drive arc 13 degrees clockwise
	mogoP(MOGO_DOWN);

	driveSet(-127, -127);                     // Back up the drive
	delay(130);
	mogoP(MOGO_DOWN - 300);                   // Bring the mobile goal up a bit
	delay(250);
	driveSet(0, 0);                           // Stop the drive

	armSettings.target = arm.sensor->average;    // Reset the arm position to it's
	                                          // current position
} /* autonLeftRed12 */

void autonLeftBlue22() {
	getMogo();                         // Get the mobile goal

	turnTo(0, 500);                    // Align to be straight
	driveSettings[1].max -= 25;        // Limit right side speed
	driveToPosition(-210, -210, 3000); // Back up
	driveSettings[1].max += 25;        // Correct speed
	turnTo(-135, 2100);                // Turn around
	placeCone();                       // Place the cone

	// Reset drive encoders & gyro
	sensorReset(drive[0].sensor);
	sensorReset(drive[1].sensor);
	sensorReset(&gyro);

	driveToPositionAngle(900,  1100, -5, 1300); // Drive arc -5 degrees
	                                            // clockwise
	TaskHandle mogoDownHandle = GO(mogoPT, MOGO_DOWN);
	driveToPositionAngle(1300, 1500, -5, 1300); // Drive arc -5 degrees
	                                            // clockwise

	// Wait for the mogo intake
	while (taskGetState(mogoDownHandle) != TASK_DEAD) {
		delay(10);            // Don't hog the CPU
	}

	driveSet(-127, -127);   // Back up the drive
	delay(130);
	mogoP(MOGO_DOWN - 300); // Bring the mobile goal up a bit
	delay(350);
	driveSet(0, 0);         // Stop the drive

	// Reset the target arm position to it's current position
	armSettings.target = arm.sensor->average;
} /* autonLeftRed22 */
