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

#define GO(task, arg)                 \
  taskCreate(&task,                   \
             TASK_DEFAULT_STACK_SIZE, \
             (void *)arg,             \
             TASK_PRIORITY_DEFAULT)

static const double inch =
  (1 /
   (3.141592653589793238462643383279502884197169399375105820974944 * 4 / 360 *
    5 /
    8))
  * 11 / 12;

/**
 * A convienence to distinguish tasks from regular functions
 */
typedef void Task;

// Sensors and the like

/**
 * Quadrature encoder in digital 1, 2
 */
extern Sensor armCoder;

/**
 * Quadrature encoder in digital 8, 9
 */
extern Sensor liftCoder;

/**
 * Drive encoders:
 *  left  @ index 0 in digital 4, 5
 *  right @ index 1 in digital 6, 7
 */
extern Sensor driveCoder[2];

/**
 * Potentiometer on the mogo manipulator in analog 1
 */
extern Sensor mogoAngle;

/**
 * Gyroscopes to measure the robot's rotation:
 *  normal     @ index 0 in analog 2
 *  redundancy @ index 1 in analog 3
 */
extern Sensor gyro;

/**
 * The ultrasonic sensor on the robot
 * echo, orange wire, in digital 3
 * ping, yellow wire, in digital 10
 */
extern Sensor sonic;

/**
 * The angle sensor on the claw @ analog 5
 */
extern Sensor clawAngle;

/**
 * The limit switch on the arm
 */
extern Sensor armLimit;

// Motors and servos

/**
 * The claw, a servo @ port 5
 */
extern Motor claw;

/**
 * The two sides of the drive:
 *  left  @ index 0 in power expander @ port 2
 *  right @ index 1 in power expander @ port 9
 */
extern Motor drive[2];

/**
 * The arm, containing:
 *  left  motor @ port    1
 *  right motor @ port    10
 */
extern Motor arm[2];

/**
 * The mogo manipulator, consisting of:
 *  left  motor   @ port   4
 *  right motor   @ port   7
 */
extern Motor mogo[2];

extern System Drive[2];
extern System Arm;
extern System Mogo;

// Stuff to set stuff
void driveSet(int l,
              int r);

/**
 * A TaskHandle for the manager task
 */
extern TaskHandle managerHandle;

/**
 * Runs the various functions to manage everything
 */
void manager(void *none);

#endif // CARL_ROBOT_H_
