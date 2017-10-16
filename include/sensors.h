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

/*
 * The different types of sensors
 */
typedef enum {
	Analog,
	AnalogPrecise,
	Digital,
	Quad,
	Sonic,
	Gyroscope,
	Placeholder,
} SensorType;

/*
 * A struct representing a Sensor of a given type
 */
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

/*
 * The handle for the sensor loop
 */
extern TaskHandle sensorLoopHandle;

/*
 * Initialize and clean up the sensors for the manager. Must be used before
 **creating any new sensors
 */
void    sensorInit();

/*
 * A task that handles the values of the sensors
 */
void    sensorLoop(void *none);

/*
 * Create a new Sensor based on the type, port, inverted, and calibration
 */
Sensor* newSensor(SensorType     type,
                  unsigned char  port,
                  bool           inverted,
                  unsigned short calibrate);

/*
 * Create a new digital sensor from it's port and inverted
 */
Sensor* newDigital(unsigned char port,
                   bool          inverted);

/*
 * Make a brand new ultrasonic sensor from the two ports
 */
Sensor* newSonic(unsigned char orange,
                 unsigned char yellow);

/*
 * Uses the given values for the ports and inverted to create a new and usable
 **quadrature encoder
 */
Sensor* newQuad(unsigned char top,
                unsigned char bottom,
                bool          inverted);

/*
 * A new analog sensor
 */
Sensor* newAnalog(unsigned char port);

#endif // CARL_SENSORS_H_
