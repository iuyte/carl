/**
 * @file rehuh.c
 * @brief A weird rerun-ish experiment
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

void autonRehuh() {
	int i = 0;
	// ///(left,  right, arm, mogo, claw, gyro) // battery
	moveTo(1353,  1401,  329, 2172, -14, 0);    // 8555 mv
	printf("\n%d\n", ++i);
	moveTo(2198,  2261,  304, 2167, -33, 1);    // 8555 mv
	printf("\n%d\n", ++i);
	moveTo(2480,  2547,  298, 23,   -27, 0);    // 8555 mv
	printf("\n%d\n", ++i);
	moveTo(2295,  2493,  298, 22,   -20, -5);   // 8555 mv
	printf("\n%d\n", ++i);
	moveTo(711,   945,   23,  22,   -28, -5);   // 8378 mv
	printf("\n%d\n", ++i);
	moveTo(-596,     -365,   23,  20,   106, -5);   // 8378 mv
	printf("\n%d\n", ++i);
	moveTo(-1783, -304,   227, 21,   107, -102); // 8378 mv
	printf("\n%d\n", ++i);
	moveTo(-1143,  444,  326, 1114, 70,  -101); // 8378 mv
	printf("\n%d\n", ++i);
	moveTo(-793,  841,  310, 1505, 69,  -101); // 8378 mv
	printf("\n%d\n", ++i);
	moveTo(-835,  781,  309, 1734, 72,  -101); // 8378 mv
	printf("\n%d\n", ++i);
	moveTo(-1325,  217,   192, 1728, 66,  -99);  // 8378 mv
	printf("\n%d\n", ++i);
	moveTo(-539,    -602,    215, 1735, 59,  46);   // 8378 mv
	printf("\n%d\n", ++i);
	moveTo(-52,   -402,   227, 2156, 39,  71);   // 8378 mv
	printf("\n%d\n", ++i);
	moveTo(905,  484,  221, 2155, 20,  76);   // 8378 mv
	printf("\n%d\n", ++i);
	moveTo(1713,  1255,  215, 64,   32,  76);   // 8378 mv
	printf("\n%d\n", ++i);
	moveTo(58,   -304,   216, 64,   54,  69);   // 8378 mv
	printf("\n%d\n", ++i);
	moveTo(-539,  458,  251, 65,   97,  -98);  // 8260 mv
	printf("\n%d\n", ++i);
	moveTo(-543,    1111,  251, 2088, 96,  -101); // 8260 mv
	printf("\n%d\n", ++i);
	moveTo(-841,  772,  844, 2188, 95,  -101); // 8260 mv
	printf("\n%d\n", ++i);
	moveTo(-874,  1242,  844, 1434, 94,  -144); // 8260 mv
	printf("\n%d\n", ++i);
	moveTo(-510,    3111,  843, 1797, 90,  -262); // 8260 mv
	printf("\n%d\n", ++i);
	moveTo(507,  4185,  843, 2176, 89,  -262); // 8260 mv
	printf("\n%d\n", ++i);
	moveTo(1128,  4798,  843, 19,   94,  -262); // 8260 mv
	printf("\n%d\n", ++i);
	moveTo(-706,  3221,  843, 18,   93,  -282); // 8260 mv
	printf("\n%d\n", ++i);
	moveTo(-991,  4381,  843, 1075, 96,  -410); // 8260 mv
	printf("\n%d\n", ++i);
	moveTo(-1105,  4508,  843, 2054, 99,  -431); // 8260 mv
	printf("\n%d\n", ++i);
	moveTo(-1342,  4272,  843, 1604, 99,  -432); // 8260 mv
	printf("\n%d\n", ++i);
	moveTo(-1900, 5534,  843, 1603, 41,  -592); // 8142 mv
	printf("\n%d\n", ++i);
	moveTo(-1310,  6361,  843, 1995, 82,  -606); // 8142 mv
	printf("\n%d\n", ++i);
	moveTo(289,   8032,  843, 2189, 85,  -607); // 8142 mv
	printf("\n%d\n", ++i);
	moveTo(1071,  8910,  843, 1280, 78,  -609); // 8142 mv
	printf("\n%d\n", ++i);
	moveTo(1885,  9404, 843, 1284, 80,  -585); // 8142 mv
	printf("\n%d\n", ++i);
	moveTo(2354,  10625, 843, 1251, 83,  -641); // 8083 mv
	printf("\n%d\n", ++i);
	moveTo(3301,  11018, 843, 1249, 29,  -590); // 8083 mv
	printf("\n%d\n", ++i);
	moveTo(3832,  11409, 843, 1755, 62,  -588); // 8083 mv
	printf("\n%d\n", ++i);
	moveTo(2861,  10513, 843, 11,   67,  -594); // 8083 mv
	printf("\n%d\n", ++i);
} /* autonRehuh */
