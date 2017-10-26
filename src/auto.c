/**
 * @file auto.c
 * @brief The primary source for the autonomous operation period
 * Copyright (C) 2017 Ethan Wells
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or(at your option) any
 * later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty ofMERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <https://www.gnu.org/licenses/>
 */

#include "../include/robot.h"

void autonomous() {
	Settings drivePID[2] = {
		newSettings(12 *inch,
								1,
		            1,
		            1,
		            &Drive[0],
		            true,
		            127,
		            -127,
		            10,
		            8,
		            5),
		newSettings(12 *inch,
								1,
		            1,
		            1,
		            &Drive[1],
		            true,
		            127,
		            -127,
		            10,
		            8,
		            5)
	};

	GO(PID, &drivePID[0]);
	GO(PID, &drivePID[1]);

	while (!drivePID[0].done || !drivePID[1].done) {
		delay(25);
	}
} /* autonomous */
