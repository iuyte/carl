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

	turnTo(-5, 300);                 // Align to a left tilt
	driveSettings[1].max -= 40;      // Limit right side speed
	driveToPosition(300, 700, 2200); // Back up
	driveSettings[1].max += 40;      // Correct speed
	turnTo(-165, 2000);              // Turn around
	placeCone();                     // Place the cone

	// Reset drive encoders & gyro
	sensorReset(drive[0].sensor);
	sensorReset(drive[1].sensor);
	sensorReset(&gyro);

	driveToPositionAngle(1000, 900, 13, 1850); // Drive arc 13 degrees clockwise
	mogoP(MOGO_DOWN);

	driveSet(-127, -127);                      // Back up the drive
	delay(130);
	mogoP(MOGO_DOWN - 300);                    // Bring the mobile goal up a bit
	delay(250);
	driveSet(0, 0);                            // Stop the drive

	armSettings.target = arm.sensor->average;     // Reset the arm position to it's
	                                           // current position
} /* autonLeftRed12 */

void autonLeftRed22() {
	getMogo(); // Get the mobile goal

	gyroSettings[0].tolerance--;
	gyroSettings[1].tolerance--;
	turnTo(-10, 500);                // Align to a left tilt
	gyroSettings[0].tolerance++;
	gyroSettings[1].tolerance++;
	driveSettings[1].max -= 40;      // Limit right side speed
	driveToPosition(388, 788, 2200); // Back up
	driveSettings[1].max += 40;      // Correct speed
	turnTo(-158, 2000);              // Turn around
	placeCone();                     // Place the cone

	// Reset drive encoders & gyro
	sensorReset(drive[0].sensor);
	sensorReset(drive[1].sensor);
	sensorReset(&gyro);

	driveToPositionAngle(1400, 1300, 13, 1800); // Drive arc 13 degrees clockwise

	turnTo(59, 750);

	sensorReset(drive[0].sensor);
	sensorReset(drive[1].sensor);
	sensorReset(&gyro);

	driveToPositionAngle(1400, 1400, 0, 1950); // Drive arc 13 degrees clockwise

	mogoP(MOGO_DOWN);

	// Wait a bit for the mobile goal to settle
	delay(250);

	TaskHandle mogoUpHandle = GO(mogoPT, MOGO_UP);
	driveSet(-127, -127); // Back up the drive
	delay(250);
	driveSet(127, 127);   // Go forward to knock mobile goal off
	delay(150);
	driveSet(-127, -127); // Back up the drive again
	delay(500);           // Make sure that the robot isn't touching a field
	                      // element
	driveSet(0, 0);       // stop the robot

	while (taskGetState(mogoUpHandle) != TASK_DEAD) {
		delay(10);
	}

	armSettings.target = arm.sensor->average; // Reset the arm position to it's
	                                       // current position
} /* autonLeftRed22 */
