/**
 * @file autoStack.c
 * @brief Drive forward and stack cones from the loader
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

void updateLinesDrive();

void autonStack() {
	getMogo();
	delay(300);
	placeCone();

	turnTo(-6, 700);
	driveToPosition(1493, 1793, 1600);
	turnTo(-73, 1300);
	driveToPosition(drivePos(0) - 550, drivePos(1) - 150, 1200);
	liftToPosition(LIFT_QUARTER, 750);
	manipSettings.target = MANIP_HOVER;

	for (size_t i = 1; i < 6; i++) {
		delay(250);
		liftSettings.target = LIFT_LOAD;

		// 4bar out
		manipToPosition(MANIP_HOVER, 1200);
		manip.power = 10;

		// set intake to in
		intake.power = 127;
		update();
		
		// drop lift to match load height
		liftToPosition(LIFT_LOAD, 300 + 50 * i);
		
		// wait to pick up the cone, then set to a hold power
		delay(200);
		intake.power = 25;
		update();

		// Bring the lift up to the stacking height
		liftToPosition(i * LIFT_CONE, i * 250);
		liftSettings.target = LIFT_DOWN + i * LIFT_CONE;

		// 4bar to stacking position
		manipToPosition(MANIP_PLACE - i * 7, 1200 + i * 30);
		liftToPosition(LIFT_DOWN + i * LIFT_CONE, 250);
		update();

		delay(300);
		manip.power = -10;
		update();
		delay(150);

		// outtake
		intake.power = -127;
		update();
		delay(500);

		// lift back up a bit
		manipSettings.target = MANIP_HOVER;
		liftToPosition(LIFT_DOWN + 260 + i * LIFT_CONE, 250);
	}

	intake.power = 0;
	resetDrive();
	return;
	
	// back up some
	driveToPosition(-400, -400, 1000);
	turnTo(-160, 3000); // turn around
	// drive forward a bit
	driveToPosition(drivePos(0) + 2350, drivePos(1) + 2000, 2600);
	
	// raise the lift
	liftSettings.target = LIFT_UP;
	mogoP(MOGO_DOWN);

	driveSet(-127, -127);                      // Back up the drive
	delay(130);
	mogoP(MOGO_DOWN - 300);                    // Bring the mobile goal up a bit
	delay(250);
	driveSet(0, 0);                            // Stop the drive

	GO(mogoPT, MOGO_DOWN);
	turnTo(45, 2500);
	resetDrive();
	driveToPosition(1500, 1500, 2800);
	mogoP(MOGO_PART);

	turnTo(-135, 3400);
} /* autonLeftRed12 */
