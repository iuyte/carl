/**
 * @file rush.c
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

#include "../include/rush.h"

Sensor *clawAngle;

void driveSetR(int l, int r) {
	motorSet(2, -l);
	motorSet(9, r);
} /* driveSet */

void armSet(int p) {
	motorSet(1,  -p);
	motorSet(10, p);
} /* armSet */

void mogoSet(int p) {
	motorSet(4, p);
	motorSet(7, -p);
} /* mogoSet */

void mogoSet2(int l, int r) {
	motorSet(4, l);
	motorSet(7, -r);
} /* mogoSet */

void clawSet(int p) {
	motorSet(5, p);
} /* clawSet */

Get driveGet(bool isLeft) {
	Get g = {
		motorGet(isLeft ? 2 : 9) * isLeft ? -1 : 1,
		driveCoder[isLeft]->value,
	};

	return g;
} /* driveGet */

Get armGet(bool isLeft) {
	Get g = {
		motorGet(isLeft ? 1 : 10)  * isLeft ? 1 : -1,
		armCoder->value,
	};

	return g;
} /* armGet */

Get mogoGet(bool isLeft) {
	Get g = {
		motorGet(isLeft ? 4 : 7)  * isLeft ? 1 : -1,
		isLeft ? mogoAngle->value : mogoAngle->redundancy->value,
	};

	return g;
} /* mogoGet */

Get clawGet() {
	Get g = {
		motorGet(5),
		clawAngle->value,
	};

	return g;
} /* clawGet */

void driveSetP(long  positionL,
               long  positionR,
               float kP,
               long  tolerance) {
	long error[2];

	do {
		error[0] = positionL - driveGet(0).position;
		error[1] = positionR - driveGet(1).position;
		driveSet(error[0] * kP, error[1] * kP);
	} while (error[0] > tolerance && error[1] > tolerance);
} /* driveSetP */

void armSetP(long  position,
             float kP,
             long  tolerance) {
	long error;

	do {
		error = position - armGet(0).position;
		armSet(error * kP);
	} while (error > tolerance);
} /* armSetP */

void mogoSetP(long  position,
              float kP,
              long  tolerance) {
	long error[2];

	do {
		error[0] = position - mogoGet(0).position;
		error[1] = position - mogoGet(1).position;
		mogoSet2(error[0] * kP, error[1] * kP);
	} while (error[0] > tolerance && error[1] > tolerance);
} /* mogoSetp */

void clawSetP(long  position,
              float kP,
              long  tolerance) {
	long error;

	do {
		error = position - clawGet().position;
		clawSet(error * kP);
	} while (error > tolerance);
} /* clawSetP */
