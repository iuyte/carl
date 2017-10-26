/**
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

#ifndef CARL_PID_H_
#define CARL_PID_H_

#include "motors.h"
#include "sensors.h"

/**
 * A structure defining a group of motors and a sensor
 */
typedef struct System {
	long    request;
	int     power;
	int     numSlaves;
	Sensor *sensor;
	Motor **slaves;
} System;

/**
 * Create and configure a new System.
 *
 * @param system a pointer to the System to configure
 * @param sensor a pointer to the Sensor of the System
 * @param num    the number of motors in the System
 * @param slaves variadic args of the motors in the System
 */
void confSystem(System *system, Sensor *sensor, int num, Motor **slaves);

/**
 * The settings for PID
 */
typedef struct Settings {
	/**
	 * The ideal position, or goal value
	 */
	long target;

	/**
	 * Maximum value to be assigned to the controlled system
	 */
	int max;

	/**
	 * Minimum value to be assigned to the controlled system
	 */
	int min;

	/**
	 * Limit for the integral value
	 */
	int iLimit;

	/**
	 * Whether or not the PID loop ends
	 */
	bool terminates;

	/**
	 * p value
	 */
	float kP;

	/**
	 * i value
	 */
	float kI;

	/**
	 * d value
	 */
	float kD;

	/**
	 * Precision for waiting on pid to reach value
	 */
	unsigned int precision;

	/**
	 * How many loop iterations to wait before breaking - only used if terminates
	 * is set to true
	 */
	unsigned int tolerance;

	/**
	 * The system the pid controls
	 */
	System *system;

	/**
	 * True when finished
	 */
	bool done;
} Settings;

/**
 * Create new settings based on the values provided
 *
 * @param target     the destination value
 * @param kP         the P value for the PID
 * @param kI         the I value for the PID
 * @param kD         the D value for the PID
 * @param system     a pointer to the system that the PID will control
 * @param terminates whether or not the PID loop terminates
 * @param max        the maximum value at which it will set the motors in the
 * system
 * @param min        the minimum value at which it will set the motors in the
 * system
 * @param iLimit     the maximum kI value
 * @param precision  the amount of precision to consider when finishing
 * @param tolerance  how many iterations of the loop before the PID loop will
 * finish - only used if terminates is true
 *
 * @returns the configured System
 */
Settings newSettings(long         target,
										 float        kP,
                     float        kI,
                     float        kD,
                     System      *system,
                     bool         terminates,
                     int          max,
                     int          min,
                     int          iLimit,
                     unsigned int precision,
                     unsigned int tolerance);

/**
 * Use the Settings to achieve the target
 *
 * @param target   the target value
 * @param settings a pointer to the settings to be used
 */
void PID(void *conf);

#endif // CARL_PID_H_
