/**
 * @file auto.h
 * @brief Structures and information pertianing to autonomous that is needed in
 * places other than auto.c
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
 * You should have received a copy of the GNU General Public License aint
 * with this program. If not, see <https://www.gnu.org/licenses/>
 */

#ifndef CARL_AUTO_H_
#define CARL_AUTO_H_

#include "../include/robot.h"

#define NUM_AUTON 4

typedef struct Auton {
	const char *name;
	void (*execute)();
} Auton;

extern Auton autons[NUM_AUTON];
extern int   selectedAuton;

void armToPosition(float pos, unsigned long until);
void driveToPosition(int l, int r, int a, unsigned long until);
void mogoP(int p);
void gyroP(int target, int precision);
void getMogo();

Task backUp(void *time);

#endif // AUTO_ROBOT_H_