/**
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

/**
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
} Sensor;

/**
 * Initialize and clean up the sensors for the manager. Must be used before
 * creating any new sensors
 */
void    sensorInit();

/**
 * A step in the management loop to handle sensors
 */
void    sensorLoop();

/**
 * Create a new Sensor
 *
 * @param type      the type of SensorType, either a Digital, Analog,
 ***AnalogPrecise, Quad, Sonic, or Gyroscope
 * @param port      the port in which the sensor in in
 * @param inverted  whether or not to invert the value
 * @param calibrate the calibration value in some cases, or anything but 0 to
 ***calibrate the Sensor
 *
 * @return a pointer to the configured Sensor
 */
Sensor* newSensor(SensorType     type,
                  unsigned char  port,
                  bool           inverted,
                  unsigned short calibrate);

/**
 * Create a new digital Sensor
 *
 * @param port     the port that the digital Sensor is in
 * @param inverted whether or not to invert the value
 *
 * @return a pointer to a configured Sensor of SensorType Digital
 */
Sensor* newDigital(unsigned char port,
                   bool          inverted);

/**
 * Make a brand new Sonic (aka ultrasonic) Sensor
 *
 * @param orange the port that the orange cable is in
 * @param yellow the port that the yellow cable is in
 *
 * @return a pointer to a configured Sensor of type Sonic
 */
Sensor* newSonic(unsigned char orange,
                 unsigned char yellow);

/**
 * Create and initialize a quadrature encoder (the red ones)
 *
 * @param top      the port that the top wire on the encoder is in
 * @param bottom   the port that the bottom wire on the encoder is in
 * @param inverted whether or not the Sensor's value should be inverted
 *
 * @return a pointer to a configured Sensor of type Quad
 */
Sensor* newQuad(unsigned char top,
                unsigned char bottom,
                bool          inverted);

/**
 * Create and configure a new analog sensor
 *
 * @param port the port that the sensor is in
 *
 * @return a pointer to the configured Sensor of type Analog
 */
Sensor* newAnalog(unsigned char port);

/**
 * Create and configure a new analog sensor
 *
 * @param port the port that the sensor is in
 *
 * @return a pointer to the configured Sensor of type Analog
 */
Sensor* newAnalogUnprecise(unsigned char port);

/**
 * Make a new gyroscope sensor
 *
 * @param port        the analog port that the gyro is plugged into
 * @param calibration the calibration of the Sensor
 */
Sensor* newGyro(unsigned char port,
                int           calibration);

/*
 * Take a Sensor's Mutex
 *
 * @param sensor the sensor from which to take the Mutex
 * @param block  the amount of milliseconds willing to wait for the Mutex to be
 * freed. -1 for indefinitely
 */
bool sensorTake(Sensor      *sensor,
                unsigned int block);

/*
 * Free a Sensor's mutex
 *
 * @param sensor the Sensor of which the mutex will be freed
 */
void sensorGive(Sensor *sensor);

/*
 * An array of Mutex guarding the sensors
 */
extern Mutex smutexes[35];

#endif // CARL_SENSORS_H_
