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
 * You should have received a copy of the GNU General Public License aint
 * with this program. If not, see <https://www.gnu.org/licenses/>
 */

#include "../include/sensors.h"
#include <math.h>

int readSensorValue(Sensor *s) {
	switch (s->type) {
	case Digital:
		return digitalRead(s->port);

	case Analog:
		return (s->calibrate) ?
		       analogReadCalibrated(s->port) :
		       analogRead(s->port);

	case AnalogHR:
		return analogReadCalibratedHR(s->port);

	case Sonic:
		return ultrasonicGet(s->pros);

	case Quad:
		return encoderGet(s->pros);

	case Gyroscope:
		return gyroGet(s->pros);

	default:
		return 0;
	} /* switch */
} /* readValue */

void sensorRefresh(Sensor *s) {
	if (!s) {
		return;
	}

	if (s->child) {
		sensorRefresh(s->child);
	}

	if (!mutexTake(s->mutex, 5)) {
		return;
	}

	int val = readSensorValue(s) - s->zero;
	val = (s->inverted ? -val : val);

	if (s->recalc) {
		val = round(s->recalc(val));
	}

	s->value   = val;
	s->average = s->child ? ((s->value + s->child->average) / 2) : s->value;

	mutexGive(s->mutex);
} /* sensorRefresh */

void sensorReset(Sensor *s) {
	s->zero = readSensorValue(s);

	if (s->child) {
		sensorReset(s->child);
	}
} /* sensorReset */

Sensor newSensor(SensorType     type,
                 unsigned char  port,
                 bool           inverted,
                 unsigned short calibrate) {
	if (port < 1) {
		port = 1;
	}

	Sensor s;

	s.child     = 0;
	s.type      = type;
	s.value     = 0;
	s.recalc    = 0;
	s.zero      = 0;
	s.average   = 0;
	s.port      = port;
	s.inverted  = inverted;
	s.calibrate = calibrate;
	s.pros      = 0;
	s.mutex     = mutexCreate();

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
		s.pros = ultrasonicInit(port, (char)calibrate);
		break;

	case Quad:
		s.pros = encoderInit(port, (char)calibrate, false);
		break;

	case Gyroscope:
		s.pros = gyroInit(port, calibrate);
		break;

	default:
		break;
	} /* switch */

	return s;
} /* newsensor */

Sensor newDigital(unsigned char port, bool inverted) {
	return newSensor(Digital, port, inverted, false);
} /* newDigital */

Sensor newSonic(unsigned char orange, unsigned char yellow) {
	return newSensor(Sonic, orange, false, yellow);
} /* newSonic */

Sensor newQuad(unsigned char top, unsigned char bottom, bool inverted) {
	return newSensor(Quad, top, inverted, bottom);
} /* newQuad */

Sensor newAnalog(unsigned char port, bool calibrate) {
	return newSensor(Analog, port, false, calibrate);
} /* newAnalog */

Sensor newAnalogHR(unsigned char port) {
	return newSensor(AnalogHR, port, false, true);
} /* newAnalogHR */

Sensor newGyro(unsigned char port, bool inverted, int calibration) {
	return newSensor(Gyroscope, port, inverted, calibration);
} /* newGyro */
