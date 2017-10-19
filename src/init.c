/**
 * @file init.c
 * @brief Perform initialization and start handler tasks
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

void initializeIO() {}

void init() {
	printf("\nInitializing... ");

	// Set up the analog sensors
	mogoAngle = newAnalog(1);

	// Set up the digital sensors
	armCoder      = newQuad(1, 2, false);
	driveCoder[0] = newQuad(4, 5, true);
	driveCoder[1] = newQuad(6, 7, false);
	liftCoder     = newQuad(8, 9, false);

	// Initialize and set up all of the motors, servos, systems, etc
	claw = newServo(5, false);
	arm  = newSystem(armCoder,
	                 newMotor(1,  true),
	                 newMotor(10, false));
	lift = newSystem(liftCoder,
	                 newMotor(3,  true),
	                 newMotor(8,  false));
	mogo = newSystem(mogoAngle,
	                 newMotor(4,  false),
	                 newMotor(7,  true));

	drive[0] = newSystem(driveCoder[0], newMotor(2, true));
	drive[1] = newSystem(driveCoder[1], newMotor(9, false));
	printf("done!\n\n");
} /* init */
