/**
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

// Motors and servos

/*
 * The claw, a servo
 */
extern Servo *claw;

/*
 * The arm, containing:
 *  left  motor @ port    9
 *  right motor @ port    2
 *  encoder     @ digital 1, 2
 */
extern System *arm;

/*
 * The mogo manipulator, consisting of:
 *  left  motor   @ port   1
 *  right motor   @ port   10
 *  potentiometer @ analog 3
 */
extern System *mogo;

/*
 * The two Systems of the drive:
 *  left  @ index 0:
 *    power expander @ port    4
 *    encoder        @ digital 4, 5
 *  right @ index 1:
 *    power expander @ port    7
 *    encoder        @ digital 6, 7
 */
extern System *drive[2];

// Sensors and the like

/*
 * Quadrature encoder in digital 1, 2
 */
extern Sensor *armCoder;

/*
 * Drive encoders:
 *  left  @ index 0 in digital 4, 5
 *  right @ index 1 in digital 6, 7
 */
extern Sensor *driveCoder[2];

/*
 * Potentiometer on the mogo manipulator in analog 3
 */
extern Sensor *mogoAngle;

// Stuff to set stuff
void driveSet(int l,
              int r);

#endif // CARL_ROBOT_H_
