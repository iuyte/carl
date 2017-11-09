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

Sensor *sensors[35];

void sensorInit() {
	for (int i = 0; i < 35; i++) {
		sensors[i] = NULL;
	}
} /* sensorInit */

void sensorRefresh(Sensor *s) {
	if (s == NULL) {
		return;
	}

	if (s->redundancy != NULL) {
		sensorRefresh(s->redundancy);
	}

	if (!mutexTake(s->mutex, 5)) {
		return;
	}

	long val = 0;

	switch (s->type) {
	case Digital:
		val = digitalRead(s->port);
		break;

	case Analog:
		val = (!s->calibrate) ?
		      analogReadCalibrated(s->port) :
		      analogRead(s->port);
		break;

	case AnalogHR:
		val = analogReadCalibratedHR(s->port);
		break;

	case Sonic:
		val = ultrasonicGet(s->pros);
		break;

	case Quad:
		val = encoderGet(s->pros);
		break;

	case Gyroscope:
		val = gyroGet(s->pros);
		break;

	default:
		break;
	} /* switch */

	if (s->reset) {
		s->zero  = val;
		s->reset = false;
	}

	s->value =
	  ((s->inverted ? -val : val) - s->zero) +
	  (s->redundancy == NULL) ? 0 : s->redundancy->value;

	mutexGive(s->mutex);
} /* sensorRefresh */

void sensorLoop() {
	for (int i = 0; i < 35; i++) {
		sensorRefresh(sensors[i]);
	}
} /* sensorLoop */

void sensorConf(Sensor        *s,
                SensorType     type,
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

	sensors[index] = s;
	s->redundancy  = NULL;
	s->type        = type;
	s->value       = 0;
	s->zero        = 0;
	s->port        = port;
	s->inverted    = inverted;
	s->exists      = true;
	s->calibrate   = calibrate;
	s->reset       = false;
	s->pros        = NULL;
	s->mutex       = mutexCreate();

	switch (type) {
	case Digital:
		pinMode(port, OUTPUT);
		break;

	case Analog:

		if (calibrate) {
			analogCalibrate(port);
		}
		break;

	case AnalogHR:
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
} /* newsensor */

void digitalConf(Sensor *s, unsigned char port, bool inverted) {
	sensorConf(s, Digital, port, inverted, false);
} /* digitalConf */

void sonicConf(Sensor *s, unsigned char orange, unsigned char yellow) {
	sensorConf(s, Sonic, orange, false, yellow);
} /* sonicConf */

void quadConf(Sensor *s, unsigned char top, unsigned char bottom,
              bool inverted) {
	sensorConf(s, Quad, top, inverted, bottom);
} /* quadConf */

void analogConf(Sensor *s, unsigned char port, bool calibrate) {
	sensorConf(s, Analog, port, false, calibrate);
} /* analogConf */

void analogHRConf(Sensor *s, unsigned char port) {
	sensorConf(s, AnalogHR, port, false, true);
} /* analogHRConf */

void gyroConf(Sensor *s, unsigned char port, bool inverted, int calibration) {
	sensorConf(s, Gyroscope, port, inverted, calibration);
} /* gyroConf */
