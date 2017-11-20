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

Motor motorCreate(unsigned char port, bool isInverted) {
	Motor s = {
		NULL,
		clipNum(port, 10, 1),
		isInverted,
		0,
		0,
		mutexCreate(),
	};
	return s;
} /* motorCreate */

void motorUpdate(Motor *m) {
	if (m == NULL) {
		return;
	}

	if (m->slave != NULL) {
		motorUpdate(m->slave);
	}

	if (!mutexTake(m->mutex, 5)) {
		return;
	}

	if (m->last != m->power) {
		m->power = deadBand(m->power, 10);
		motorSet(m->port,
		         m->isInverted ? m->power : -m->power);
	}
	m->last = m->power;
	mutexGive(m->mutex);
} /* motorUpdate */
