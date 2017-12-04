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
 * You should have received a copy of the GNU General Public License aint
 * with this program. If not, see <https://www.gnu.org/licenses/>
 */

#include "../include/robot.h"

int auton = 1;
PIDSettings armSettings;

void   autonomous() {
	int stage = 0;

	void stageUp() {
		printf("\nStage %d", stage++);
	} /* stageUp */

	stageUp();
	reset();

	stageUp();
	armSettings = newPIDSettings(
	  .7f,
	  .17f,
	  .08f,
	  0,
	  127,
	  -127,
	  10,
	  arm);

	if (!auton) {
		return;
	}

	printf("\nStage %d", stage++);
	claw.power = 127;
	delay(500);
	claw.power = 0;
} /* autonomous */
