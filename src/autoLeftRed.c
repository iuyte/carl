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
	TaskHandle backUpHandle = NULL; // GO(backUp, millis());

	getMogo();

	// Drop cone
	claw.power = 127;
	motorUpdate(&claw);
	delay(400);
	claw.power = 0;

	// Align to a left tilt
	turnTo(-5, 300);
	// Limit right side speed
	driveSettings[1].max -= 40;
	// Back up
	driveToPosition(300, 700, 1700);
	// Correct speed
	driveSettings[1].max += 40;

	// Turn around
	// turnTo(180, 1500);

	// Reset drive encoders
	sensorReset(&driveCoder[0]);
	sensorReset(&driveCoder[1]);

	driveToPositionAngle(700, 780, -5, 1200);
	mogoP(MOGO_DOWN);

	driveSet(-127, -127);
	delay(250);
	mogoP(MOGO_DOWN - 100);
	delay(500);
	driveSet(0, 0);

	// Cleanup of tasks
	if (backUpHandle) {
		taskDelete(backUpHandle);
	}

	armSettings.target = armCoder.average;
	while (isAutonomous()) {
		PID(&armSettings);
		update();
		delay(10);
	}
} /* autonLeftRed12 */
