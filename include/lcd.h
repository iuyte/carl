/*
 * @file lcd.h
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

#ifndef CARL_LCD_H_
#define CARL_LCD_H_

#include "API.h"

/*
 * A struct defining an LCD Menu
 */
typedef struct Menu {
	char        *text[2];
	struct Menu *parent;
	struct Menu *prev;
	struct Menu *next;
	struct Menu *(*onSelect)();
} Menu;

/*
 * The root menu for LCDs
 */
static Menu lcdRoot;

/*
 * Defines an LCD
 */
typedef struct LCD {
	PROS_FILE *port;
	Menu      *currentMenu;
} LCD;

/*
 * Handles the lcdManager
 */
extern TaskHandle lcdManagerHandle;
void lcdManager(void *none);

#endif // CARL_LCD_H_
