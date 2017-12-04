/**
 * @file pid.c
 * @brief A PID implementation
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

#include "../include/pid.h"

PIDSettings newPIDSettings(float  kP,
                           float  kI,
                           float  kD,
                           float  target,
                           int    max,
                           int    min,
                           int    iLimit,
                           Motor *root) {
	PIDSettings s = {
		kP,
		kI,
		kD,
		target,
		max,
		min,
		iLimit,
		root,
		0,
		0,
		0,
		0,
	};

	return s;
} /* newPIDSettings */

void PID(PIDSettings *settings) {
	float error;
	float power;

	settings->current = settings->root->sensor->value;
	error             = settings->target - settings->current;

	settings->integral =
	  (settings->kI != 0 && abs((int)error) < settings->iLimit) ?
	  (settings->integral + error) :
	  clipNum(settings->integral + error, settings->iLimit, -settings->iLimit);

	settings->derivative = error - settings->error;
	settings->error      = error;

	power =
	  clipNum(
	    (settings->kP * error) +
	    (settings->kI * settings->integral) +
	    (settings->kD * settings->derivative),
	    settings->max,
	    settings->min);

	mutexTake(settings->root->mutex, -5);
	settings->root->power = (settings->root->power + (int)(power + 0.5)) / 2;
	mutexGive(settings->root->mutex);
} /* PID */
