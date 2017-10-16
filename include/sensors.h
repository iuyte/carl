/*
 * @file sensors.h
 * @brief Hardware abstraction for sensors
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

#ifndef CARL_SENSORS_H_
#define CARL_SENSORS_H_

#include "API.h"

typedef enum {
	Analog,
	AnalogPrecise,
	Digital,
	Quad,
	Sonic,
	Gyroscope,
	Placeholder,
} SensorType;

typedef struct Sensor {
	SensorType     type;
	long           value;
	long           zero;
	bool           inverted;
	bool           reset;
	unsigned short calibrate;
	unsigned char  port;
	bool           exists;
	void          *pros;
} Sensor;

extern TaskHandle sensorLoopHandle;

void    sensorInit();
void    sensorLoop(void *none);

Sensor* newSensor(SensorType     type,
                  unsigned char  port,
                  bool           inverted,
                  unsigned short calibrate);
Sensor* newDigital(unsigned char port,
                   bool          inverted);
Sensor* newSonic(unsigned char orange,
                 unsigned char yellow);
Sensor* newQuad(unsigned char top,
                unsigned char bottom,
                bool          inverted);
Sensor* newAnalog(unsigned char port);

#endif // CARL_SENSORS_H_
