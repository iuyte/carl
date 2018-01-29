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
	getMogo(); // Get the mobile goal

	gyroSettings[0].tolerance--;
	gyroSettings[1].tolerance--;
	turnTo(-10, 575);                // Align to a left tilt
	gyroSettings[0].tolerance++;
	gyroSettings[1].tolerance++;
	driveSettings[1].max -= 40;      // Limit right side speed
	GO(placeConeT, NULL);            // Place cone
	driveToPosition(388, 788, 2200); // Back up
	driveSettings[1].max += 40;      // Correct speed
	turnTo(-158, 2000);              // Turn around

	// Reset drive encoders & gyro
	sensorReset(drive[0].sensor);
	sensorReset(drive[1].sensor);
	sensorReset(&gyro);

	driveToPositionAngle(1400, 1300, 13, 1800); // Drive arc 13 degrees clockwise

	// turnTo(59, 750);

	sensorReset(drive[0].sensor);
	sensorReset(drive[1].sensor);
	sensorReset(&gyro);

	driveToPositionAngle(1200, 1200, 0, 1550); // Drive straight
	sensorReset(&gyro);

	mogoP(MOGO_DOWN);                          // Drop mobile goal

	driveSet(50, 50);
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

	armSettings.target = arm.sensor->average;             // Reset the arm
	                                                      // position to
	                                                      // it's
	                                                      // current position

	turnTo(170, 2000);                                    // TURN AROUND,
	mogoP(MOGO_DOWN);                                     // DROP A MOGO INTAKE
	                                                      // AND
	sensorReset(drive[0].sensor);
	sensorReset(drive[1].sensor);
	driveToPositionAngle(1750, 1750, gyro.average, 2200); // DRIVE A LITTLE BIT,
	mogoP(MOGO_UP);
	driveToPosition(0, 0, 1200);                          // BACK UP.
	turnTo(0, 2000);                                      // TUUUURN AROUUUUUNNND
	sensorReset(drive[0].sensor);
	sensorReset(drive[1].sensor);
	driveToPosition(400, 400, 1200);
	mogoP(MOGO_DOWN);
	driveSet(-127, -127);
	delay(350);
	mogoUpHandle = GO(mogoPT, MOGO_UP);
	delay(350);
	driveSet(0, 0);
} /* autonLeftRed22 */
