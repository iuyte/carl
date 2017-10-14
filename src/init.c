/*
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

	motorInit();
	sensorInit();

	claw        = newServo(6, false);
	arm.left    = newMotor(9, true);
	arm.right   = newMotor(2, false);
	mogo.left   = newMotor(1, false);
	mogo.right  = newMotor(10, true);
	drive.left  = newMotor(4,  true);
	drive.right = newMotor(7,  false);

	motorLoopHandle = taskCreate(&motorLoop,
	                             TASK_DEFAULT_STACK_SIZE,
	                             NULL,
	                             TASK_PRIORITY_DEFAULT + 1);
	sensorLoopHandle = taskCreate(&sensorLoop,
	                              TASK_DEFAULT_STACK_SIZE,
	                              NULL,
	                              TASK_PRIORITY_DEFAULT + 1);
	printf("done!\n\n");
} /* initialize */
