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
	getMogo();
	placeCone();

	turnTo(-6, 700);
	driveToPosition(1450, 1750, 1600);
	turnTo(-65, 1300);
	driveToPosition(drivePos(0) - 600, drivePos(1) - 200, 1200);
	liftToPosition(LIFT_QUARTER, 750);

	for (size_t i = 1; i < 13; i++) {
		// 4bar out
		manipToPosition(MANIP_HOVER, 1200);
		manip.power = 14;

		// set intake to in
		intake.power = 127;
		update();
		
		// drop lift to match load height
		liftToPosition(LIFT_LOAD, 300 + 50 * i);
		
		// wait to pick up the cone, then set to a hold power
		delay(250);
		intake.power = 25;
		update();

		// Bring the lift up to the stacking height
		liftToPosition(LIFT_DOWN + i * LIFT_CONE, 350 + i * 375);

		// 4bar to stacking position
		manipToPosition(MANIP_PLACE, 1200);
		update();

		delay(300);

		// outtake
		intake.power = -127;
		manip.power = -5;
		update();
		delay(500);

		// lift back up a bit
		liftToPosition(LIFT_DOWN + 250 + i * LIFT_CONE, 250);
	}

	intake.power = 0;
} /* autonLeftRed12 */
