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
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <https://www.gnu.org/licenses/>
 */

#include "../include/pid.h"

void confSystem(System *system, Sensor *sensor, int num, Motor **slaves) {
	System s = {
		0,
		num,
		sensor,
		slaves,
	};

	*system = s;
} /* newSystem */

Settings newSettings(float        target,
                     float        kP,
                     float        kI,
                     float        kD,
                     System      *system,
                     bool         terminates,
                     int          max,
                     int          min,
                     int          iLimit,
                     unsigned int precision,
                     unsigned int tolerance) {
	Settings s;

	s.target     = target;
	s.kP         = kP;
	s.kI         = kI;
	s.kD         = kD;
	s.system     = system;
	s.terminates = terminates;
	s.max        = max;
	s.min        = min;
	s.iLimit     = iLimit;
	s.precision  = precision;
	s.tolerance  = tolerance;
	s.isDone     = false;
	s.isActive   = true;

	return s;
} /* newSettings */

void PID(void *conf) {
	Settings *settings = (Settings *)conf;
	float     current;
	float     error;
	float     lastError = 0;
	float     integral  = 0;
	float     derivative;
	float     power;
	bool      success[5];
	Motor   **slaves = settings->system->slaves;

	settings->isDone = false;

	do {
		delay(25);

		current = settings->system->sensor->value;
		error   = settings->target - current;

		if ((unsigned int)abs((int)error) <= settings->precision) {
			for (int i = 0; i < 5; i++) {
				if (success[i]) {
					continue;
				}
				success[i] = true;
				break;
			}

			if ((success[4]) && settings->terminates) {
				settings->isDone = true;
			}
			continue;
		} else {
			for (int i = 0; i < 5; i++) {
				success[i] = false;
			}
		}
		integral = (settings->kI != 0 && abs((int)error) < settings->iLimit)
		           ? (integral + error)
							 : 0;
		derivative = error - lastError;
		lastError  = error;
		power      =
		  (settings->kP * error) +
		  (settings->kI * integral) + (settings->kD * derivative);
		power = clipNum(power /* 8.1f / powerLevelMain() */,
		                settings->max,
		                settings->min);

		if (settings->isActive) {
			settings->system->power = (int)power;
		}

		for (int m = 0; m < settings->system->num; m++) {
			slaves[m]->power = settings->system->power;
		}
	} while (!settings->isDone);
} /* PID */
