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

	while (true) {
		if (lcdState == 0x100) {
			if (selectedAuton < 1) {
				selectedAuton = NUM_AUTON - 1;
			} else {
				selectedAuton -= 1;
			}

			while (lcdReadButtons(uart1) == 0x100) {
				delay(15);
			}
		} else if (lcdState == 0x001) {
			if (selectedAuton > NUM_AUTON - 1) {
				selectedAuton = 0;
			} else {
				selectedAuton += 1;
			}

			while (lcdReadButtons(uart1) == 0x001) {
				delay(15);
			}
		}

		if (!isEnabled()) {
			update();
		}

		info();
		lcdSetText(uart1, 1, autons[selectedAuton].name);
		lcdState = lcdReadButtons(uart1);
		delay(25);
	}
} /* selectAuton */
