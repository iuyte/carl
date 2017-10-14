/*
 * @file pid.h
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

#ifndef CARL_PID_H
#define CARL_PID_H

#include "motors.h"
#include "sensors.h"
#include <stdarg.h>

typedef void *System;
System newSystem(Sensor *sensor,
                 Motor  *slaves,
                 ...);

typedef struct Settings {
	/*
	 * Maximum value to be assigned to the controlled system
	 */
	int max;

	/*
	 * Minimum value to be assigned to the controlled system
	 */
	int min;

	/*
	 * Limit for the integral value
	 */
	int iLimit;

	/*
	 * Whether or not the PID loop ends
	 */
	bool terminates;

	/*
	 * p value
	 */
	float kP;

	/*
	 * i value
	 */
	float kI;

	/*
	 * d value
	 */
	float kD;

	/*
	 * Precision for waiting on pid to reach value
	 */
	unsigned int precision;

	/*
	 * The system the pid controls
	 */
	System system;
} Settings;

Settings newSettings(float        kP,
                     float        kI,
                     float        kD,
                     System       system,
                     bool         terminates,
                     int          max,
                     int          min,
                     int          iLimit,
                     unsigned int precision);

void PID(long      target,
         Settings *settings);

#endif // CARL_PID_H
