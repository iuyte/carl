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
	sensorReset(&driveCoder[0]);
	sensorReset(&driveCoder[1]);
	sensorReset(&gyro);

	driveToPositionAngle(1000, 900, 13, 1850); // Drive arc 13 degrees clockwise
	mogoP(MOGO_DOWN);

	driveSet(-127, -127);                      // Back up the drive
	delay(130);
	mogoP(MOGO_DOWN - 300);                    // Bring the mobile goal up a bit
	delay(250);
	driveSet(0, 0);                            // Stop the drive

	armSettings.target = armCoder.average;     // Reset the arm position to it's
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
	sensorReset(&driveCoder[0]);
	sensorReset(&driveCoder[1]);
	sensorReset(&gyro);

	driveToPositionAngle(1400, 1300, 13, 1800); // Drive arc 13 degrees clockwise

	turnTo(59, 750);

	sensorReset(&driveCoder[0]);
	sensorReset(&driveCoder[1]);
	sensorReset(&gyro);

	driveToPositionAngle(1400, 1400, 0, 1950); // Drive arc 13 degrees clockwise

	mogoP(MOGO_DOWN);

	// Wait a bit for the mobile goal to settle
	delay(250);

	TaskHandle mogoUpHandle = GO(mogoPT, MOGO_UP);
	driveSet(-127, -127); // Back up the drive
	delay(250);
	driveSet(127, 127);
	delay(150);
	driveSet(-127, -127); // Back up the drive
	delay(500);
	driveSet(0, 0);

	while (taskGetState(mogoUpHandle) != TASK_DEAD) {
		delay(10);
	}

	armSettings.target = armCoder.average; // Reset the arm position to it's
	                                       // current position
} /* autonLeftRed22 */

void autonLeftRed22Old() {
	getMogo(); // Get the mobile goal

	gyroSettings[0].tolerance--;
	gyroSettings[1].tolerance--;
	turnTo(-9, 1500);                  // Align to a left tilt
	gyroSettings[0].tolerance++;
	gyroSettings[1].tolerance++;
	driveSettings[1].max -= 25;        // Limit right side speed
	driveToPosition(-450, -400, 3300); // Back up
	driveSettings[1].max += 25;        // Correct speed
	turnTo(-133, 2200);                // Turn around
	placeCone();                       // Place the cone

	// Reset drive encoders & gyro
	sensorReset(&driveCoder[0]);
	sensorReset(&driveCoder[1]);
	sensorReset(&gyro);

	driveToPositionAngle(1250, 900,  5, 1400);          // Drive arc 5 degrees
	                                                    // clockwise
	TaskHandle mogoDownHandle = GO(mogoPT, MOGO_DOWN);
	driveToPositionAngle(1500, 1300, 5, 1600);          // Drive arc 5 degrees
	                                                    // clockwise

	while (taskGetState(mogoDownHandle) != TASK_DEAD) { // Wait for the mogo
		// intake
		delay(10);                                        // Don't hog the CPU
	}

	driveSet(-127, -127);                               // Back up the drive
	delay(130);
	mogoP(MOGO_DOWN - 300);                             // Bring the mobile goal
	                                                    // up a bit
	delay(350);
	driveSet(0, 0);                                     // Stop the drive

	armSettings.target = armCoder.average;              // Reset the arm position
	                                                    // to it's
	                                                    // current position
} /* autonLeftRed22 */
