/**
 * @file autoMeat.c
 * @brief The autonomous for placing an animal in the carnivore bin
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

void autoMeat(Direction d);

void autoMeatLeft() {
	autoMeat(dLeft);
} /* autoMeatLeft */

void autoMeatRight() {
	autoMeat(dRight);
} /* autoMeatRight */

void autoMeat(Direction d) {
	for (size_t i = 0; i < 2; i++) {
		driveSettings[i].max = -(driveSettings[i].min = -30);
		gyroSettings[i].max  = -(gyroSettings[i].min  = -35);
	}

	turnTo(20 * d, 1500);
	resetDrive();
	
	driveToPosition(-1500, -1500, 3500);
	turnTo(0, 1500);
	resetDrive();

	driveSettings[0].max = -(driveSettings[0].min = -25);
	driveSettings[1].max = -(driveSettings[1].min = -25);

	driveToPosition(-2850, -2850, 7950);

	driveSet(-15, -15);
	delay(150);

	//for (int i = drive[0].power, j = drive[1].power; i < 0 && j < 0; ++i + j++) {
	//	driveSet(i, j);
	//	update();
	//	delay(75);
	//}

	driveSet(0, 0);
	delay(300);

	driveSet(-15 * d, 15 * d);
	delay(300);

	turnTo(-90 * d, 2300);
	resetDrive();

	for (size_t i = 0; i < 2; i++) {
		driveSettings[i].max = -(driveSettings[i].min = -127);
		gyroSettings[i].max  = -(gyroSettings[i].min  = -127);
	}

	driveSet(-127, -127);
	delay(275);
	driveToPosition(-520, -520, 1850);
}
