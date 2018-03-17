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

#define PI \
  3.1415926535897932384626433832795028841971693993751058209749445923078
#define DRIVE_WHEEL_DIAMETER 4.10
#define DRIVE_ENCODER_RATIO 1.6
#define LCD_PORT uart1
#define drivePos(index) drive[index].sensor->value

#define GO(task, arg)                 \
  taskCreate(&task,                   \
             TASK_DEFAULT_STACK_SIZE, \
             (void *)(arg),           \
             TASK_PRIORITY_DEFAULT)

extern double inch;

// Sensors and the like

/**
 * Gyroscopes to measure the robot's rotation:
 *  left          in analog 1
 *  right @ child in analog 2
 */
extern Sensor gyro;

/**
 * The limit switch on the arm
 *  in  @ digital 12
 *  out @ digital 11
 */
extern Sensor armLimit[2];

/**
 * Ultrasonic sensor
 *  orange @ port 6,
 *	yellow @ port 7
 */
extern Sensor *sonic;

/**
 * The three line sensors, from left->right, in ports 6, 7, and 8
 */
extern Sensor line[3];

// Motors and servos

/**
 * The claw, a motor @ port 3
 *  sensor pot       @ analog 5
 */
extern Motor claw;

/**
 * The two sides of the drive:
 *  left  @ index 0 in power expander @ port 2
 *    child center motor              @ port 4
 *    sensor                          @ digital 4, 5
 *  right @ index 1 in power expander @ port 9
 *    child center motor              @ port 7
 *    sensor                          @ digital 8, 9
 */
extern Motor drive[2];

/**
 * The arm, containing:
 *  left motor            @ port 5
 *    child right motor   @ port 6
 *      child upper motor @ port 8
 *    sensor quad         @ digital 1, 2
 */
extern Motor arm;

/**
 * The mogo manipulator, consisting of:
 *  left  motor  @ port 1
 *    sensor pot @ analog 3
 *  right motor  @ port 10
 *    sensor pot @ analog 4
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
 * PID settings for the claw
 */
extern PIDSettings clawSettings;

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

/**
 * Reset the left and right drive encoders
 */
void resetDrive();

bool waitForDriveStall(unsigned long blockTime);

#endif // CARL_ROBOT_H_
