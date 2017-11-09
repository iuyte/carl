/**
 * @file Sensors.h
 * @brief Hardware abstraction for Sensors
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

/**
 * The different types of Sensors
 */
typedef enum {
	Analog,
	AnalogHR,
	Digital,
	Quad,
	Sonic,
	Gyroscope,
	Placeholder,
} SensorType;

/**
 * A struct representing a Sensor of a given type
 */
typedef struct Sensor {
	struct Sensor *redundancy;
	SensorType     type;
	long           value;
	long           zero;
	bool           inverted;
	bool           reset;
	unsigned short calibrate;
	unsigned char  port;
	bool           exists;
	void          *pros;
	Mutex          mutex;
} Sensor;

/**
 * Initialize and clean up the Sensors for the manager. Must be used before
 * creating any new Sensors
 */
void sensorInit();

/**
 * A step in the management loop to handle Sensors
 */
void sensorLoop();

/**
 * Configure a new Sensor
 *
 * @param type      the type of SensorType, either a Digital, Analog,
 * AnalogHR, Quad, Sonic, or Gyroscope
 * @param port      the port in which the Sensor in in
 * @param inverted  whether or not to invert the value
 * @param calibrate the calibration value in some cases, or anything but 0 to
 * calibrate the Sensor
 */
void sensorConf(Sensor        *s,
                SensorType     type,
                unsigned char  port,
                bool           inverted,
                unsigned short calibrate);

/**
 * Configure a new digital Sensor
 *
 * @param s        the Sensor to configure
 * @param port     the port that the digital Sensor is in
 * @param inverted whether or not to invert the value
 */
void digitalConf(Sensor *s,
		unsigned char port,
                   bool          inverted);

/**
 * Configure a Sonic (aka ultrasonic) Sensor
 *
 * @param s      the Sensor to configure
 * @param orange the port that the orange cable is in
 * @param yellow the port that the yellow cable is in
 */
void sonicConf(Sensor *s, unsigned char orange,
                 unsigned char yellow);

/**
 * Configure and initialize a quadrature encoder (the red ones)
 *
 * @param s        the Sensor to configure
 * @param top      the port that the top wire on the encoder is in
 * @param bottom   the port that the bottom wire on the encoder is in
 * @param inverted whether or not the Sensor's value should be inverted
 */
void quadConf(Sensor *s, unsigned char top,
                unsigned char bottom,
                bool          inverted);

/**
 * Configure a new analog Sensor
 *
 * @param s         the Sensor to configure
 * @param port      the port that the Sensor is in
 * @param calibrate whether or not to calibrate the sensor
 */
void analogConf(Sensor *s, unsigned char port, bool calibrate);

/**
 * Configure a new analog HR sensor
 *
 * @param s    the Sensor to configure
 * @param port the port that the Sensor is in
 */
void analogHRConf(Sensor *s, unsigned char port);

/**
 * Configure a gyroscope Sensor
 *
 * @param s           the Sensor to configure
 * @param port        the analog port that the gyro is plugged into
 * @param inverted    whether or not the gyroscope is inverted
 * @param calibration the calibration of the Sensor
 */
void gyroConf(Sensor *s, unsigned char port, bool inverted, int calibration);

#endif // CARL_SENSORS_H_
