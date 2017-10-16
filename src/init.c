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

void initialize() {
	printf("\nInitializing... ");

	// Clean up everything before we touch it. Otherwise, what we do next will be
	// overwritten
	motorInit();
	sensorInit();

	// Set up the sensors
	armCoder      = newQuad(1, 2, false);
	mogoAngle     = newAnalog(3);
	driveCoder[0] = newQuad(4, 5, true);
	driveCoder[1] = newQuad(6, 7, false);

	// Initialize and set up all of the motors, servos, systems, etc
	claw = newServo(6, false);
	arm  = newSystem(armCoder,
	                 newMotor(9,  true),
	                 newMotor(2,  false));
	mogo = newSystem(mogoAngle,
	                 newMotor(1,  false),
	                 newMotor(10, true));

	drive[0] = newSystem(driveCoder[0], newMotor(4, true));
	drive[1] = newSystem(driveCoder[1], newMotor(7, false));

	// Create manager tasks to make life easy
	motorLoopHandle = taskCreate(&motorLoop,
	                             TASK_DEFAULT_STACK_SIZE,
	                             NULL,
	                             TASK_PRIORITY_DEFAULT + 1);
	sensorLoopHandle = taskCreate(&sensorLoop,
	                              TASK_DEFAULT_STACK_SIZE,
	                              NULL,
	                              TASK_PRIORITY_DEFAULT + 1);
	systemLoopHandle = taskCreate(&systemLoop,
	                              TASK_DEFAULT_STACK_SIZE,
	                              NULL,
	                              TASK_PRIORITY_DEFAULT + 1);
	printf("done!\n\n");
} /* initialize */
