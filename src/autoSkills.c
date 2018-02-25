/**
 * @file autoSkills.c
 * @brief Programming skills
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

#include "../include/auto.h"

void autonLeftRed22();

void autonSkills() {
	// Get the mobile goal using the left red 22 point auton routine
	autonLeftRed22();

	// ///////////////////////////////////////////////////////////////// 22 POINTS

	sensorReset(drive[0].sensor);
	sensorReset(drive[1].sensor);

	// Back up a small amount
	driveToPosition(-325, -325, 500);
	driveSet(35, 35);
	delay(450);
	sensorReset(&gyro);

	driveToPosition(-500, -500, 750);
	turnTo(132, 2500);     // TURN AROUND,
	GO(mogoPT, MOGO_DOWN); // DROP A MOGO INTAKE;
	delay(450);

	// Reset sensors
	sensorReset(drive[0].sensor);
	sensorReset(drive[1].sensor);
	driveToPositionAngle(600, 600, 135, 1250);
	turnTo(202, 2500);     // TURN AROUND,

	// Reset sensors
	sensorReset(drive[0].sensor);
	sensorReset(drive[1].sensor);

	driveToPositionAngle(1650, 1650, 209, 2900);

	// Mogo intake up
	mogoP(MOGO_UP);
	turnTo(373, 2000); // TUUUURN AROUUUND

	// Reset sensors
	sensorReset(drive[0].sensor);
	sensorReset(drive[1].sensor);

	driveToPositionAngle(1300, 1400, 368, 1876);
	// Bring mogo intake to middle
	TaskHandle mogoHandle = GO(mogoPT, MOGO_MID);
	driveToPositionAngle(2375, 2475, 361, 2750);
	sensorReset(&gyro);
	if (taskGetState(mogoHandle))
		taskDelete(mogoHandle);
	mogoP(MOGO_DOWN);
	driveSet(-64, -64);
	delay(250);
	mogoHandle = GO(mogoPT, MOGO_MID);
	driveToPosition(2250, 2250, 1800);

	sensorReset(drive[0].sensor);
	sensorReset(drive[1].sensor);

	// ///////////////////////////////////////////////////////////////// 32 POINTS

	turnTo(-120, 3400);
	// Reset sensors
	sensorReset(drive[0].sensor);
	sensorReset(drive[1].sensor);
	driveToPositionAngle(900, 900, -120, 1850);
	turnTo(-176, 2500);     // TURN AROUND,

	// Reset sensors
	sensorReset(drive[0].sensor);
	sensorReset(drive[1].sensor);

	mogoHandle = GO(mogoPT, MOGO_DOWN);
	delay(300);
	driveToPositionAngle(1430, 1430, -175, 2600);

	// Mogo intake up
	mogoP(MOGO_UP);
	driveToPositionAngle(750, 750, -176, 1200);
	turnTo(-361, 2000); // TUUUURN AROUUUND

	// Reset sensors
	sensorReset(drive[0].sensor);
	sensorReset(drive[1].sensor);

	driveToPositionAngle(900, 800, -357, 1876);
	// Bring mogo intake to middle
	mogoHandle = GO(mogoPT, MOGO_MID);
	driveToPositionAngle(1400, 1300, -351, 2200);
	sensorReset(&gyro);
	sensorReset(drive[0].sensor);
	sensorReset(drive[1].sensor);
	if (taskGetState(mogoHandle))
		taskDelete(mogoHandle);
	mogoP(MOGO_DOWN);
	driveSet(72, 72);
	delay(300);
	sensorReset(&gyro);
	sensorReset(drive[0].sensor);
	sensorReset(drive[1].sensor);
	driveSet(-64, -64);
	delay(250);
	mogoHandle = GO(mogoPT, MOGO_MID);
	driveToPosition(-200, -200, 750);

	sensorReset(drive[0].sensor);
	sensorReset(drive[1].sensor);

	// ///////////////////////////////////////////////////////////////// 42 POINTS

	// Back up a small amount
	driveToPositionAngle(-200, -235, 5, 800);
	GO(mogoPT, MOGO_MID); // Bring mogo intake to middle

	//  Turn around to aim for the red mogo across the field
	turnTo(184, 3000);
	sensorReset(drive[0].sensor);
	sensorReset(drive[1].sensor);
	// Drive across the field
	driveToPositionAngle(600,  600,  188, 1200);
	// Mogo intake down to pick it up
	mogoHandle = GO(mogoPT, MOGO_DOWN);
	driveToPositionAngle(3300, 3200, 184, 3900);
	mogoHandle = GO(mogoPT, MOGO_MID + 100);
	turnTo(174, 750);
	driveToPositionAngle(4050, 3750, 172, 3400);

	sensorReset(drive[0].sensor);
	sensorReset(drive[1].sensor);

	// Reset drive encoders & gyro
	sensorReset(drive[0].sensor);
	sensorReset(drive[1].sensor);

	mogoHandle = GO(mogoPT, MOGO_MID + 125);
	driveToPositionAngle(1525, 1425, 13, 1675); // Drive arc 13 degrees clockwise
	
	mogo.power = 127;
	motorUpdate(&mogo);
	driveSet(64, 64);
	delay(200);
	driveSet(-80, -80);
	delay(150);
	
	if (taskGetState(mogoHandle))
		taskDelete(mogoHandle);
	mogoHandle = GO(mogoPT, MOGO_MID - 100);
	driveToPosition(935, 935, 1800);

	sensorReset(drive[0].sensor);
	sensorReset(drive[1].sensor);

	// ///////////////////////////////////////////////////////////////// 62 POINTS
} /* autonLeftRed22 */
