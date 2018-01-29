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
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <https://www.gnu.org/licenses/>
 */

#ifndef CARL_AUTO_H_
#define CARL_AUTO_H_

#include "../include/robot.h"

#define MAX_AUTON 9

enum MOGO_POS {
	MOGO_UP = 75,
	MOGO_DOWN = 2125,
};

enum ARM_POS {
	ARM_DOWN = 10,
	ARM_QUARTER = 275,
	ARM_HALF = 430,
	ARM_3_QUARTER = 500,
};


typedef struct Auton {
	const char *name;
	void (*execute)();
} Auton;

typedef enum Direction {
	dUp,
	dDown,
	dLeft,
	dRight,
	dIn,
	dOut,
} Direction;

extern Auton autons[MAX_AUTON + 1];
extern int   selectedAuton;

void armToPosition(float pos, unsigned long until);
void driveToPosition(int l, int r, unsigned long until);
void driveToPositionAngle(int l, int r, int a, unsigned long until);
void mogoP(int p);
void gyroPID(int target, int precision);
void turnTo(int angle, unsigned long until);

void getMogo();
void placeCone();

Task backUp(void *time);
Task mogoPT(void *p);
Task placeConeT(void *none);

#endif // AUTO_ROBOT_H_
