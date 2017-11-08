/**
 * @file sensors.c
 * @brief Implementation of hardware abstraction for sensors
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

#include "../include/sensors.h"

Sensor sensors[35];
Mutex  smutexes[35];

void sensorInit() {
	Sensor s = {
		NULL,
		Placeholder,
		0,
		0,
		false,
		false,
		0,
		0,
		false,
		NULL,
	};

	for (int i = 0; i < 35; i++) {
		sensors[i]  = s;
		smutexes[i] = mutexCreate();
	}
} /* sensorInit */

void sensorLoop() {
	for (int i = 0; i < 35; i++) {
		if (!sensors[i].exists || !mutexTake(smutexes[i], 5)) {
			continue;
		}
		long val = 0;

		switch (sensors[i].type) {
		case Digital:
			val = digitalRead(sensors[i].port);
			break;

		case Analog:
			val = (!sensors[i].calibrate) ?
			                   analogReadCalibrated(sensors[i].port) :
			                   analogRead(sensors[i].port);
			break;

		case AnalogPrecise:
			val = analogReadCalibratedHR(sensors[i].port);
			break;

		case Sonic:
			val = ultrasonicGet(sensors[i].pros);
			break;

		case Quad:
			val = encoderGet(sensors[i].pros);
			break;

		case Gyroscope:
			val = gyroGet(sensors[i].pros);
			break;

		default:
			break;
		} /* switch */

		if (sensors[i].reset) {
			sensors[i].zero  = val;
			sensors[i].reset = false;
		}

		sensors[i].value =
		  (sensors[i].inverted ? -val : val) -
		  sensors[i].zero;

		mutexGive(smutexes[i]);
	}
} /* sensorLoop */

Sensor* newSensor(SensorType     type,
                  unsigned char  port,
                  bool           inverted,
                  unsigned short calibrate) {
	if (port < 1) {
		port = 1;
	}
	int index;

	if ((type == Digital) || (type == Sonic) || (type == Quad)) {
		index = port - 1;
	} else {
		index = port + BOARD_NR_GPIO_PINS - 1;
	}
	mutexTake(smutexes[index], -1);
	Sensor *s = &sensors[index];

	s->type      = type;
	s->value     = 0;
	s->zero      = 0;
	s->port      = port;
	s->inverted  = inverted;
	s->exists    = true;
	s->calibrate = calibrate;
	s->reset     = false;
	s->pros      = NULL;

	switch (type) {
	case Digital:
		pinMode(port, OUTPUT);
		break;

	case Analog:

		if (calibrate) {
			analogCalibrate(port);
		}
		break;

	case AnalogPrecise:
		  analogCalibrate(port);
		break;

	case Sonic:
		s->pros = ultrasonicInit(port, (char)calibrate);
		break;

	case Quad:
		s->pros = encoderInit(port, (char)calibrate, false);
		break;

	case Gyroscope:
		s->pros = gyroInit(port, calibrate);
		break;

	default:
		break;
	} /* switch */

	mutexGive(smutexes[index]);
	return s;
} /* newSensor */

Sensor* newDigital(unsigned char port,
                   bool          inverted) {
	return newSensor(Digital, port, inverted, false);
} /* newDigital */

Sensor* newSonic(unsigned char orange,
                 unsigned char yellow) {
	return newSensor(Sonic, orange, false, yellow);
} /* newSonic */

Sensor* newQuad(unsigned char top,
                unsigned char bottom,
                bool          inverted) {
	return newSensor(Quad, top, inverted, bottom);
} /* newQuad */

Sensor* newAnalog(unsigned char port) {
	return newSensor(Analog, port, false, true);
} /* newAnalog */

Sensor* newAnalogUnprecise(unsigned char port) {
	return newSensor(Analog, port, false, false);
} /* newAnalogUnprecise */

Sensor* newGyro(unsigned char port, int calibration) {
	return newSensor(Gyroscope, port, false, calibration);
} /* newGyro */

bool sensorTake(Sensor *sensor, unsigned int block) {
	return sensorTake(smutexes[(sensor->type == Digital || sensor->type ==
	                            Quad ||
	                            sensor->type == Sonic) ? (sensor->port -
	                                                      1) : (sensor->port +
	                                                            BOARD_NR_GPIO_PINS
	                                                            - 1)], block);
} /* sensorTake */

void sensorGive(Sensor *sensor) {
	mutexGive(smutexes[(sensor->type == Digital || sensor->type == Quad ||
	                    sensor->type == Sonic) ? (sensor->port -
	                                              1) : (sensor->port +
	                                                    BOARD_NR_GPIO_PINS
	                                                    - 1)]);
} /* sensorGive */
