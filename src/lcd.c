/**
 * @file lcd.c
 * @brief Useful for interacting with LCD displays. Menus and the like
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

#include "../include/lcd.h"

TaskHandle LCDHandle;

Task lcdTask(void *none) {
	unsigned int lcdState = 0x000;
	unsigned int newLcdState = 0x000;

	while (true) {
		newLcdState = lcdReadButtons(uart1);
		if (lcdState != newLcdState) {
			lcdState = newLcdState;
			if (lcdState == 4) {
				if (selectedAuton < 1) {
					selectedAuton = MAX_AUTON;
				} else {
					selectedAuton -= 1;
				}
			} else if (lcdState == 1) {
				if (selectedAuton == MAX_AUTON) {
					selectedAuton = 0;
				} else {
					selectedAuton += 1;
				}
			} else if (lcdState == 3) {
				if (autons[selectedAuton].sensor != NULL)
					sensorReset(*(autons[selectedAuton].sensor));
			} else if (lcdState == 7) {
				exit(0);
			}
		}

		if (!isEnabled()) {
			update();
		}

		info();
		lcdPrint(uart1, 1, "b:%u, a:%s",
		         powerLevelMain(),
						 autons[selectedAuton].name);
		lcdPrint(uart1, 2, "%s: %d",
		         autons[selectedAuton].sensorName,
		         (*autons[selectedAuton].sensor)->average);
		delay(25);
	}
} /* selectAuton */
