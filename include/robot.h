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
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
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
#include "lcd.h"

#define PI 3.1415926535897932384626433832795028841971693993751058209749445923078
#define DRIVE_WHEEL_DIAMETER 4.10
#define DRIVE_ENCODER_RATIO 1.6
#define LCD_PORT uart1

#define GO(task, arg)                 \
  taskCreate(&task,                   \
             TASK_DEFAULT_STACK_SIZE, \
             (void *)arg,             \
             TASK_PRIORITY_DEFAULT)

extern double inch;

// Sensors and the like

/**
 * Quadrature encoder in digital 1, 2
 */
extern Sensor armCoder;

/**
 * Drive encoders:
 *  left  @ index 0 in digital 4, 5
 *  right @ index 1 in digital 8, 9
 */
extern Sensor driveCoder[2];

/**
 * Potentiometer on the mogo manipulator
 *  left  in Analog 3
 *  right in Analog 4
 */
extern Sensor mogoAngle[2];

/**
 * Gyroscopes to measure the robot's rotation:
 *  left          in analog 1
 *  right @ child in analog 2
 */
extern Sensor gyro;

/**
 * The ultrasonic sensor on the robot
 *  echo, orange wire, in digital 3
 *  ping, yellow wire, in digital 10
 */
extern Sensor sonic;

/**
 * The angle sensor on the claw in analog 5
 */
extern Sensor clawAngle;

/**
 * The limit switch on the arm
 *  in  @ digital 12
 *  out @ digital 11
 */
extern Sensor armLimit[2];

// Motors and servos

/**
 * The claw, a motor @ port 3
 */
extern Motor claw;

/**
 * The two sides of the drive:
 *  left  @ index 0 in power expander @ port 2
 *    child center motor              @ port 4
 *  right @ index 1 in power expander @ port 9
 *    child center motor              @ port 7
 */
extern Motor drive[2];

/**
 * The arm, containing:
 *  left  motor       @ port 5
 *  	child right motor @ port 6
 * 	Quad encoder in digital 1, 2
 */
extern Motor arm;

/**
 * The mogo manipulator, consisting of:
 *  left  motor @ port 1
 *  right motor @ port 10
 */
extern Motor mogo;

/**
 * PID settings for the arm
 */
extern PIDSettings armSettings;

/**
 * PID settings for the drive
 *  left  @ index 0
 *  right @ index 1
 */
extern PIDSettings driveSettings[2];

/**
 * PID settings for the gyro on the drive
 *  left  @ index 0
 *  right @ index 1
 */
extern PIDSettings gyroSettings[2];

/**
 * Prints information and sets the LCD line 2 to display battery voltage
 */
void info();

// Stuff to set stuff
void driveSet(int l,
              int r);

/**
 * Reset the sensors on the robot
 */
void reset();

/**
 * Update motors and refresh sensors
 */
void update();

#endif // CARL_ROBOT_H_
