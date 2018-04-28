/**
 * @file autoTest.c
 * @brief autonomous mode for testing
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

void autonTest() {
	update();
	driveSet(35, 35);
	int angle;

	Mutex m = angleFromUpcomingLine(&angle, NULL, 5000);
	mutexTakeDelete(m, -1);
	#ifdef DEBUG_MODE
		printf("\n\n//\n\rA:%d\n\\\\\n\n", angle);
	#endif

	gyro.zero = angle;
	gyro.child->zero = angle;
	driveSet(0, 0);
	// turnTo(0, 2500);
} /* autonLeftRed12 */
