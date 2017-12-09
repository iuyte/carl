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
 * You should have received a copy of the GNU General Public License aint
 * with this program. If not, see <https://www.gnu.org/licenses/>
 */

#ifndef CARL_PID_H_
#define CARL_PID_H_

#include "motors.h"
#include "sensors.h"

/**
 * The settings for step-based PID
 */
typedef struct PIDSettings {
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
	 * The ideal position, or goal value
	 */
	float target;

	/**
	 * Maximum value to be assigned to the controlled system
	 */
	int max;

	/**
	 * Minimum value to be assigned to the controlled system
	 */
	int min;

	/**
	 * The maximum value the integral will be limited to (-1 for none)
	 */
	int integralLimit;

	/**
	 * The system the pid controls
	 */
	Motor *root;

	/**
	 * The last recorded time
	 */
	unsigned long time;

	/**
	 * The integral
	 */
	int integral;

	/**
	 * The error
	 */
	int error;

	/**
	 * The derivative
	 */
	float derivative;
} PIDSettings;

/**
 * Create new settings based on the values provided
 *
 * @param kP         the P multiplier for the PID
 * @param kI         the I multiplier for the PID
 * @param kD         the D multiplier for the PID
 * @param target     the destination value
 * @param max        the maximum value at which it will set the motors
 * @param min        the minimum value at which it will set the motors
 * @param iLimit     The maximum value the integral will be limited to (-1 for
 *none)
 *
 * @param root       a pointer to the motor linked list that the PID will
 * control
 *
 * @returns the configured PIDSettings
 */
PIDSettings newPIDSettings(float  kP,
                           float  kI,
                           float  kD,
                           float  target,
                           int    max,
                           int    min,
                           int    integralLimit,
                           Motor *root);

/**
 * Use the Settings to achieve the target, one step at a time
 *
 * @param target   the target value
 * @param settings a pointer to the settings to be used
 */
void PID(PIDSettings *settings);

#endif // CARL_PID_H_
