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

void initLoop();

int auton = 1;

void autonomous() {
	int stage = 0;

	printf("\nStage %d", stage++);
	initLoop();
	delay(25);

	if (!auton) {
		return;
	}

	// switch (false) {
	// case true:
	// printf("\nStage %d", stage++);
	// armSetP(550, .7, 15);
	// hold();
	//
	//    printf("\nStage %d", stage++);
	//    driveSetP(-3.15 * 12 * inch, -3.15 * 12 * inch, .5, 10);
	//    driveSet(17, 17);
	//    delay(500);
	//    armSetP(800, .7, 15);
	//
	//    printf("\nStage %d", stage++);
	//    clawSet(-127);
	//    delay(500);
	//    clawSet(0);

// 	printf("\nStage %d", stage++);
	// armSetP(500, .7, 15);
// 	delay(500);
// 
// 	printf("\nStage %d", stage++);
// 	armSetP(0, 1, 100);
// 	driveSetP(-12 * inch, -12 * inch, .8, 100);

	// break;

	// default:

	printf("\nStage %d", stage++);
	clawSet(127);
	delay(500);
	clawSet(50);

	printf("\nStage %d", stage++);
	armSetP(800, .7, 15);
	armSet(-11);

	printf("\nStage %d", stage++);
	driveSetP(-13 * inch, -13 * inch, .5, 10);
	delay(500);

	printf("\nStage %d", stage++);
	clawSet(-127);
	delay(500);
	clawSet(0);
	delay(500);

	// break;

	// } /* switch */
} /* autonomous */
