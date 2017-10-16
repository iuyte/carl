/*
 * @file robot.h
 * @brief General things related to the robot
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

#ifndef CARL_ROBOT_H_
#define CARL_ROBOT_H_

#include "motors.h"
#include "sensors.h"
#include "pid.h"

/*
 * Motors and servos
 */
extern Servo  *claw;
extern System *arm;
extern System *mogo;
extern System *drive[2];

/*
 * Sensors and the like
 */
extern Sensor *armCoder;
extern Sensor *driveCoder[2];
extern Sensor *mogoAngle;

/*
 * Stuff to set stuff
 */
void driveSet(int l,
              int r);

#endif // CARL_ROBOT_H_
