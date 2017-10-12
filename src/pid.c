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

#include "../include/pid.h"

typedef void *System;

typedef struct System_hid {
	Sensor *sensor;
	Motor **slaves;
} System_hid;

System newSystem(Sensor *sensor, int num, Motor *slaves, ...) {
	System_hid s;

	s.sensor = sensor;
	s.slaves = (Motor **)malloc(sizeof(Motor *) * num);

	va_list motors;
	va_start(motors, slaves);

	for (int i = 0; i < num; i++) {
		s.slaves[i] = va_arg(motors, Motor *);
	}
	System_hid *sy = (System_hid *)malloc(sizeof(System_hid));
	*sy = s;
	return (System)(void *)sy;
} /* newSystem */
