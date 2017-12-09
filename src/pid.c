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
#include <math.h>

PIDSettings newPIDSettings(float  kP,
                           float  kI,
                           float  kD,
                           float  target,
                           int    max,
                           int    min,
                           int    integralLimit,
                           Motor *root) {
	PIDSettings s = {
		kP,
		kI,
		kD,
		target,
		max,
		min,
		integralLimit,
		root,
		millis(),
		0,
		0,
		0,
	};

	return s;
} /* newPIDSettings */

void PID(PIDSettings *settings) {
	float error;
	float power;

	error = settings->target - settings->root->sensor->average;

	settings->integral  += error / (millis() - settings->time);
	settings->time       = millis();
	settings->derivative = error - settings->error;
	settings->error      = error;

	power = clipNum(
	  (settings->kP * error) +
	  (settings->kI * (settings->integralLimit == -1) ? settings->integral :
	   clipNum(settings->integral, settings->integralLimit,
	           -settings->integralLimit)) +
	  (settings->kD * settings->derivative),
	  settings->max,
	  settings->min);

	mutexTake(settings->root->mutex, -5);

	settings->root->power = round(power);
	mutexGive(settings->root->mutex);
} /* PID */
