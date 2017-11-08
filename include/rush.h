/**
 * @file rush.h
 * @brief Low-quality code for when you don't have time to make stuff work
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

#ifndef CARL_RUSH_H_
#define CARL_RUSH_H_

#include "robot.h"

/**
 * The angle sensor on the claw @ analog 5
 */
extern Sensor *clawAngle;

typedef void Task;

typedef struct Get {
	int  power;
	long position;
} Get;

void driveSetR(int l,
               int r);
void armSet(int p);
void mogoSet(int p);
void mogoLockSet(bool isLocked);
void clawSet(int p);
void hold();

void driveSetP(long  positionL,
               long  positionR,
               float kP,
               long  tolerance);
void armSetP(long  position,
             float kP,
             long  tolerance);
void mogoSetP(long  position,
              float kP,
              long  tolerance);
void clawSetP(long  position,
              float kP,
              long  tolerance);

Get  driveGet(bool isLeft);
Get  armGet(bool isLeft);
Get  mogoGet(bool isLeft);
Get  clawGet();

bool armLimit();

typedef struct pSettings {
	long          position;
	float         kP;
	long          tolerance;
	unsigned long delay;
	long          positionR;
} pSettings;

Task driveSetPT(void *settings);
Task armSetPT(void *settings);
Task mogoSetPT(void *settings);
Task clawSetPT(void *settings);
Task mogoLockSetT(void *isLocked);
Task clawPT(void *position);

#endif // CARL_RUSH_H
