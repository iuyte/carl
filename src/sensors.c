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

Sensor    sensors[24];

void sensorInit() {
	Sensor s = {
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

	for (int i = 0; i < 24; i++) {
		sensors[i] = s;
	}
} /* sensorInit */

void sensorLoop() {
		for (int i = 0; i < 24; i++) {
			if (!sensors[i].exists){
				continue;
			}

			switch (sensors[i].type) {
			case Digital:
				sensors[i].value = digitalRead(sensors[i].port);
				break;

			case Analog:
				sensors[i].value = (!sensors[i].calibrate) ?
				                   analogReadCalibrated(sensors[i].port) :
				                   analogRead(sensors[i].port);
				break;

			case AnalogPrecise:
				sensors[i].value = analogReadCalibratedHR(sensors[i].port);
				break;

			case Sonic:
				sensors[i].value = ultrasonicGet(sensors[i].pros);
				break;

			case Quad:
				sensors[i].value = encoderGet(sensors[i].pros);
				break;

			case Gyroscope:
				sensors[i].value = gyroGet(sensors[i].pros);
				break;

			default:
				break;
			} /* switch */
			sensors[i].value =
			  (sensors[i].inverted ? -sensors[i].value : sensors[i].value) -
			  sensors[i].zero;

			if (sensors[i].reset) {
				sensors[i].zero  = sensors[i].value;
				sensors[i].reset = false;
			}
		}
} /* sensorLoop */

Sensor* newSensor(SensorType     type,
                  unsigned char  port,
                  bool           inverted,
                  unsigned short calibrate) {
	if (port < 1) {
		port = 1;
	}
	Sensor *s = &sensors[port - 1];

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

Sensor* newGyro(unsigned char port, int calibration) {
	return newSensor(Gyroscope, port, false, calibration);
} /* newGyro */
