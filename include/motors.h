/**
 * @file motors.h
 * @brief Motor structure and a motor handler
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

#ifndef CARL_MOTORS_H_
#define CARL_MOTORS_H_
#include "API.h"

#define clipNum(input, high, low) (input > high) ? high : (input < \
                                                           low) ? low : input
#define sng(input) (input > 0) ? 1 : (input < 0) ? -1 : 0
#define deadBand(input, dead) (abs(input) > dead) ? input : input

/**
 * A motor structure, containing a motor's port, invertation, and power
 */
typedef struct Motor {
	unsigned char port;
	bool          isInverted;
	int           power;
	int           last;
} Motor;

/**
 * Initialize the motor arrays
 */
void   motorInit();

/**
 * A step in the management loop
 */
void   motorLoop();

/**
 * Create a new Motor
 *
 * @param port     the port that the motor is in
 * @param isInverted whether or not the motor is isInverted
 *
 * @return A pointer to the Motor in memory
 */
Motor* newMotor(unsigned char port,
                bool          isInverted);

/*
 * An array of Mutex that protects each port
 */
extern Mutex mmutexes[10];

#endif // CARL_MOTORS_H_
