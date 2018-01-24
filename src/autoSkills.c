/**
 * @file autoSkills.c
 * @brief Programming skills
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

void autonSkills() {
	getMogo();                         // Get the mobile goal

	turnTo(-5, 525);                   // Align to a left tilt
	driveSettings[1].max -= 25;        // Limit right side speed
	driveToPosition(-200, -200, 3000); // Back up
	driveSettings[1].max += 25;        // Correct speed
	turnTo(-135, 2000);                // Turn around
	placeCone();                       // Place the cone

	// Reset drive encoders & gyro
	sensorReset(drive[0].sensor);
	sensorReset(drive[1].sensor);
	sensorReset(&gyro);

	driveToPositionAngle(900,  1100, -5, 1300);         // Drive arc -5 degrees
	TaskHandle mogoDownHandle = GO(mogoPT, MOGO_DOWN);
	driveToPositionAngle(1300, 1500, -5, 1300);         // Drive arc -5 degrees

	while (taskGetState(mogoDownHandle) != TASK_DEAD) { // Wait for the mogo drop
		delay(10);                                        // Don't hog the CPU
	}

	sensorReset(&gyro);

	// Start the drive backwards
	driveSet(-127, -127);
	delay(150); // Wait a bit

	// Start the mobile goal intake moving up bit
	mogoDownHandle = GO(mogoPT, MOGO_DOWN - 300);

	// Return to previous position
	driveToPositionAngle(0, 0, 0, 2500);

	turnTo(120, 1000);                       // Turn to angle to the right
	sensorReset(drive[0].sensor);            // Reset the drive encoders
	sensorReset(drive[1].sensor);

	GO(mogoPT, MOGO_DOWN);                   // Drop the mogo intake
	driveToPosition(300, 300, 1200);         // Drive forward a bit
	turnTo(210, 1000);                       // Turn to fully face the mobile goal

	sensorReset(drive[0].sensor);            // Reset the drive encoders
	sensorReset(drive[1].sensor);

	driveToPositionAngle(940, 900, 6, 2000); // Get the mogo
	mogoP(MOGO_UP);

	// Reset the arm's target position to it's current position
	armSettings.target = arm.sensor->average;
} /* autonLeftRed22 */
