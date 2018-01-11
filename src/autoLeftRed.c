/**
 * @file leftRed.c
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
	TaskHandle backUpHandle = GO(backUp, millis());

	getMogo();
	TaskHandle placingCone = GO(placeCone, NULL);
	driveToPosition(500, 500, 0, 1700);
	turnTo(180, 1500);
	sensorReset(drive[0].sensor);
	sensorReset(drive[1].sensor);

	driveToPosition(750, 780, -3, 1000);
	mogoP(MOGO_DOWN);

	driveSet(-127, -127);
	delay(250);
	mogoP(MOGO_UP);

	// Cleanup of tasks
	if (placingCone) {
		taskDelete(placingCone);
	}

	if (backUpHandle) {
		taskDelete(backUpHandle);
	}

	while (isAutonomous()) {
		PID(&armSettings);
		update();
		delay(10);
	}
} /* autonLeftRed12 */
