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

	turnTo(-6, 500);                 // Align to a left tilt
	driveSettings[1].max -= 40;      // Limit right side speed
	GO(placeConeT, NULL);
	driveToPosition(220, 650, 2400); // Back up
	driveSettings[1].max += 40;      // Correct speed
	turnTo(-165, 2000);              // Turn around
	delay(400);

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

	armSettings.target = arm.sensor->average;  // Reset the arm position to it's
	                                           // current position
} /* autonLeftRed12 */

void autonLeftRed22() {
	getMogo();                         // Get the mobile goal

	turnTo(-12, 1000);                 // Align to a left tilt
	driveSettings[1].max -= 40;        // Limit right side speed
	GO(placeConeT, NULL);              // Place cone
	driveToPosition(-455, -180, 5500); // Back up
	driveSettings[1].max += 40;        // Correct speed
	turnTo(-160, 2300);                // Turn around

	// Reset drive encoders & gyro
	sensorReset(drive[0].sensor);
	sensorReset(drive[1].sensor);
	sensorReset(&gyro);

	TaskHandle mogoHandle = GO(mogoPT, MOGO_MID + 125);
	driveToPositionAngle(1525, 1425, 13, 1675); // Drive arc 13 degrees clockwise
	
	mogo.power = 127;
	motorUpdate(&mogo);
	driveSet(64, 64);
	delay(200);
	driveSet(-80, -80);
	delay(150);
	
	if (taskGetState(mogoHandle))
		taskDelete(mogoHandle);
	mogoHandle = GO(mogoPT, MOGO_MID - 100);
	driveToPosition(890, 890, 2000);

	while (taskGetState(mogoHandle)) {
		delay(10);
	}

	armSettings.target = arm.sensor->average; // Reset the arm position to it's
	                                          // current position
} /* autonLeftRed22 */
