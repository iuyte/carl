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
 * ANY WARRANTY; without even the implied warranty ofMERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <https://www.gnu.org/licenses/>
 */

#include "../include/lcd.h"

const char *autonNames[2] = {
	"turn left",
	"turn right",
};

void displayMenu(PROS_FILE *port, LCDMenu menu) {
	lcdPrint(port, 1, "%s: ", menu.name);
	lcdPrint(port, 2, "%d",   menu.sensor->value);
} /* displayMenu */

void update();
void info();

void selectAuton(void *storeSelect) {
	/*
	 *   PROS_FILE *storage;
	 *
	 *   if (storage = fopen("auton", "r")) {
	 * *storeSelected = fgetc(storage);
	 *        fclose(storage);
	 *   }
	 */
	int *storeSelected = (int *)storeSelect;

	unsigned int lcdState = 0x000;

	do {
		delay(25);

		update();
		info();
		lcdPrint(uart1, 1, "Ato: %s", autonNames[*storeSelected]);

		switch (lcdState) {
		case 0x100:
			*storeSelected = 0;
			break;

		case 0x001:
			*storeSelected = 1;
			break;

		default:
			break;
		} /* switch */

		/*
		 *   if (storage = fopen("auton", "w")) {
		 *        fputc(*storeSelected, storage);
		 *        fclose(storage);
		 *   }
		 */
	} while (true);
} /* selectAuton */
