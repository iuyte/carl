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
#define deadBand(input, dead) (abs(input) > dead) ? input : 0

typedef struct Motor {
	unsigned char port;
	bool          inverted;
	int           power;
} Motor;

typedef struct Servo {
	unsigned char port;
	bool          inverted;
	int           position;
} Servo;

extern TaskHandle motorLoopHandle;

void   motorInit();
void   motorLoop(void *none);
Motor* newMotor(unsigned char port,
                bool          inverted);
Servo* newServo(unsigned char port,
                bool          inverted);

#endif // CARL_MOTORS_H_
