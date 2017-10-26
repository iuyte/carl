/**
 * @file motors.c
 * @brief Implements the Motor type and the motor handler
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

#include "../include/motors.h"

Motor motors[10];

void motorInit() {
	for (int i = 0; i < 10; i++) {
		motors[i].power      = 0;
		motors[i].port       = 1;
		motors[i].isInverted = false;
	}
} /* motorPreLoop */

void motorLoop() {
	for (int i = 0; i < 10; i++) {
		if (motors[i].port == 0) {
			continue;
		}

		if (motors[i].last != motors[i].power) {
			motorSet(motors[i].port,
			         motors[i].isInverted ? motors[i].power : -motors[i].power);
		}
		motors[i].last = motors[i].power;
	}
} /* motorLoop */

Motor* newMotor(unsigned char port, bool isInverted) {
	Motor *m = &motors[port - 1];

	m->port       = port;
	m->power      = 0;
	m->last       = 0;
	m->isInverted = isInverted;

	return m;
} /* newMotor */
