/**
 * @file robot.c
 * @brief More general things related to the robot
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

#include "../include/robot.h"

#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"

Motor *claw;
Motor *drive[2];
Motor *arm[2];
Motor *mogo[2];

Sensor *armCoder;
Sensor *liftCoder;
Sensor *driveCoder[2];
Sensor *mogoAngle;
Sensor *gyro[2];
Sensor *sonic;

System Drive[2];
System Arm;
System Mogo;

TaskHandle managerHandle;

void init();
void initLoop();
void info() {
//	printf(
//	  RED " |  %4ld     | " GREEN "%4ld    | " YELLOW "%4ld    | " \
//		BLUE "%4ld    | " CYAN "%4ld    | " RED "%3ld    | " GREEN " %4ld    | " \
//		YELLOW "%4u mv | " RESET "\n",
//	  (long)(driveCoder[0]->value / inch),
//	  (long)(driveCoder[1]->value / inch),
//	  armCoder->value,
//	  mogoAngle->value,
//	  clawAngle->value,
//	  gyros(),
//	  sonic->value,
//	  powerLevelMain());
//	lcdPrint(uart1, 2, "%u mV", powerLevelMain());
	printf(
	  GREEN " |  %4ld     | " "%4ld    | " "%4ld    | " \
		"%4ld    | " "%4ld    | " "%3ld    | " " %4ld    | " \
		"%4u mv | " "\n",
	  (long)(driveCoder[0]->value / inch),
	  (long)(driveCoder[1]->value / inch),
	  armCoder->value,
	  mogoAngle->value,
	  clawAngle->value,
	  gyros(),
	  sonic->value,
	  powerLevelMain());
	lcdPrint(uart1, 2, "%u mV", powerLevelMain());
} /* info */

bool initialized = false;

void driveSet(int l, int r) {
	drive[0]->power = l;
	drive[1]->power = r;
} /* driveSet */

void initialize() {
	// Clean up everything before we touch it. Otherwise, what we do next will be
	// overwritten
	sensorInit();

	print(
"MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMNds/:ohNMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n" \
"MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMmy+:/oys+:-/ydMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n" \
"MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMNhs/+shmMMMmmmdyo/:ohNMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n" \
"MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMmyooydNMMMMMho+smmmmmdhs++sdMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n" \
"MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMNhyshmMMMMMMMMMMy/:ommmmmmmmmmdyoshNMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n" \
"MMMMMMMMMMMMMMMMMMMMMMMMMMMMMmdddNMMMMMMMMMMMMMMMMmmmmmmmmmmmmmmmmdhydmMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n" \
"MMMMMMMMMMMMMMMMMMMMMMMMMMNyyyhhhhhmMMMMMMMMMMMMMMmmmmmmmmmmmmmmdssssssosNMMMMMMMMMMMMMMMMMMMMMMMMMM\n" \
"MMMMMMMMMMMMMMMMMMMMMMMMMMMd/::::::/yhhhhhhhhhhhhhyyyyyyyyyyyyys:------:dMMMMMMMMMMMMMMMMMMMMMMMMMMM\n" \
"MMMMMMMMMMMMMMMMMMMMMMMMMMMMNo::::::::::::::::::::--------------------+NMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n" \
"MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMhso+oosyyhddddddddddyyyyyyyyyyysso++/+oyMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n" \
"MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMmhmhs/::::/sMMMMMMdymmmmmmo::---:+shymMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n" \
"MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMmdshNNmy+::oMMMMMMmhmmmmmm+--:ohmmyodhMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n" \
"MMMMMMMMMMMMMMMMMMMMMMMMMMMMMh+::yh+dMMN/:oMMMMMMNdmmmmmmo--hmmh+yh:-:yNMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n" \
"MMMMMMMMMMMMMMMMMMMMMMMMMMNy/:::::om+omMh:oMMMMMMdymmmmmmo-smdo/do-----:omMMMMMMMMMMMMMMMMMMMMMMMMMM\n" \
"MMMMMMMMMMMMMMMMMMMMMMMMms:::::::::/dy:yNssMMMMMMNdmmmmmmo/ds:od/---------+hMMMMMMMMMMMMMMMMMMMMMMMM\n" \
"MMMMMMMMMMMMMMMMMMMMMMN+:::::::::::::sd//hyMMMMMMdymmmmmmss/:hy-------------/mMMMMMMMMMMMMMMMMMMMMMM\n" \
"MMMMMMMMMMMMMMMMMMMMMMMms:::::::::::::+mo:omMMMMMdymmmmmd+-/d+------------:smMMMMMMMMMMMMMMMMMMMMMMM\n" \
"MMMMMMMMMMMMMMMMMMMMMMMMMNy/:::::::::::/hy::yNMMMdymmmms:-sh:-----------/yNMMMMMMMMMMMMMMMMMMMMMMMMM\n" \
"MMMMMMMMMMMMMMMMMMMMMMMMMMMMd+:::::::::::sd/:/dMMdymmh/-:ds-----------+dMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n" \
"MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMms::::::::::+do::oNdydo--+d/---------:omMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n" \
"MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMNy/:::::::::hh::/so:-:yh:--------:yNMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n" \
"MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMh+::::::::om+://-/do--------+hMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n" \
"MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMmo:::::::/ds:-od/-------omMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n" \
"MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMNy/::::::ydhy------:sNMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n" \
"MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMh+:::::++-----/hMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n" \
"MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMdo::::----odMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n" \
"MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMNs/:-:sNMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n" \
"MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMNhhNMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n" \
"MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n" \
"MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n" \
"MN.........--:+smMMMMMMMMMM/.........-:/oyNMMMMMMMMMMMMMMNho/-....:/sdMMMMMMMMMMMMMMMdo/-.``.-:+sdMM\n" \
"MN               -dMMMMMMMM-               /NMMMMMMMMMMh:             `+mMMMMMMMMMMh.             `m\n" \
"MN     :ssss+.    `mMMMMMMM-    .sssss/`    .NMMMMMMMN/      -+oo/.     `sMMMMMMMMm     .syso/.   hM\n" \
"MN     oMMMMMN.    sMMMMMMM-    -MMMMMMm     hMMMMMMM:     +mMMMMMMd-     sMMMMMMMh     -hNMMMMNhdMM\n" \
"MN     oMMMMMN`    sMMMMMMM-    -MMMMMMm     hMMMMMMh     +MMMMMMMMMM.    `MMMMMMMM-       `-/shNMMM\n" \
"MN     :ssso/`    .NMMMMMMM-    .sssso:     :MMMMMMMs     hMMMMMMMMMM/     mMMMMMMMMy/`          /mM\n" \
"MN               /NMMMMMMMM-              `oMMMMMMMMd     :MMMMMMMMMm`    .MMMMMMMMNMMMNdyo/.     `m\n" \
"MN     .:::::/ohNMMMMMMMMMM-    `::::     dMMMMMMMMMM+     -hMMMMMNs`     hMMMMMMMd`-odMMMMMM+     s\n" \
"MN     oMMMMMMMMMMMMMMMMMMM-    -MMMMo     sMMMMMMMMMMs`     `-::-      .dMMMMMMMd`     .://:     `N\n" \
"MN     oMMMMMMMMMMMMMMMMMMM-    -MMMMMs     /NMMMMMMMMMNo.            :yMMMMMMMMMNs:`            /mM\n" \
"MM+++++hMMMMMMMMMMMMMMMMMMMs++++oMMMMMMy+++++sMMMMMMMMMMMMmho+////+shNMMMMMMMMMMMMMMMmyo+////+shNMMM\n" \
"MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n" \
"MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n" \
"MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n" \
"MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n");

	// motorInit();

	// Call the init function to do more stuff
	if (!initialized) {
		init();
	}
	initLoop();

	// Start the manager that manages everything basiccally
	managerHandle = taskCreate(&manager,
	                           TASK_DEFAULT_STACK_SIZE,
	                           NULL,
	                           TASK_PRIORITY_DEFAULT + 1);

	// Wait for initialization to end
	while (!isAutonomous() && !isEnabled()) {
		delay(15);
	}
} /* initialize */

void manager(void *none) {
	while (true) {
		// motorLoop();
		sensorLoop();
		info();
		delay(10);
	}
} /* manager */

long gyros() {
	return (gyro[0]->value + gyro[1]->value) / 2;
} /* gyros */
