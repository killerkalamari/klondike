/*
Klondike Solitaire for Casio fx-9750GIII
Copyright (C) 2022  Jeffry Johnston

This file is part of Klondike Solitaire.

Klondike Solitaire is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Klondike Solitaire is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Klondike Solitaire.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <gint/keyboard.h>
#include <gint/usb-ff-bulk.h>
#include "kbd.h"

#define KEY_EXE    21
#define KEY_7      65
#define KEY_EXIT  116
#define KEY_RIGHT 118
#define KEY_SHIFT 129
#define KEY_OPTN  130
#define KEY_LEFT  133
#define KEY_F1    145
#define KEY_F2    146
#define KEY_F3    147
#define KEY_F5    149
#define KEY_F6    150

#define KEY_SLIM_EXE     5
#define KEY_SLIM_ACON   23
#define KEY_SLIM_EXIT   49
#define KEY_SLIM_F2     97
#define KEY_SLIM_OPTN  100
#define KEY_SLIM_DOWN  101
#define KEY_SLIM_RIGHT 102
#define KEY_SLIM_F3    113
#define KEY_SLIM_HELP  117
#define KEY_SLIM_POWER 148
#define KEY_SLIM_LEFT  149

void kbd_init(void)
{
	usb_interface_t const *interfaces[] = { &usb_ff_bulk, NULL };
	usb_open(interfaces, GINT_CALL_NULL);
}

/*
How to take an in-game screenshot:
1. Run: fxlink -iw
2. Connect the calculator via USB then press EXIT (don't press F1 - F3)
3. Press "7" in the game
*/
static void take_screenshot(void)
{
	if (usb_is_open())
		usb_fxlink_screenshot(1);
}

command_t kbd_game_input(void)
{
	while (1) {
		switch (getkey_opt(0xCC, NULL).key) {
		case KEY_OPTN:
		case KEY_SLIM_OPTN:
			return COMMAND_OPTIONS_MENU;
		case KEY_F1: /* KEY_SLIM_F1 */
		case KEY_SLIM_DOWN:
			return COMMAND_FLIP_TO_WASTE;
		case KEY_F6: /* KEY_SLIM_UP */
			return COMMAND_AUTO_FOUNDATIONS;
		case KEY_LEFT:
		case KEY_SLIM_LEFT:
			return COMMAND_PREVIOUS_MOVE;
		case KEY_RIGHT:
		case KEY_SLIM_RIGHT:
			return COMMAND_NEXT_MOVE;
		case KEY_SHIFT:
		case KEY_EXE:
		case KEY_SLIM_POWER:
		case KEY_SLIM_EXE:
			return COMMAND_MOVE;
		case KEY_SLIM_HELP:
			return COMMAND_HELP_ENG_SLIM;
		case KEY_SLIM_ACON:
			return COMMAND_TERMINATE;
		case KEY_7:
			// Take screenshot
			take_screenshot();
			break;
		}
	}
}

command_t kbd_options_input(void)
{
	while (1) {
		switch (getkey_opt(0xCC, NULL).key) {
		case KEY_F1: /* KEY_SLIM_F1 */
			return COMMAND_NEW_GAME;
		case KEY_F2:
		case KEY_SLIM_F2:
			return COMMAND_FLIP_1;
		case KEY_F3:
		case KEY_SLIM_F3:
			return COMMAND_FLIP_3;
		case KEY_F5:
			return COMMAND_HELP_ENG;
		case KEY_F6:
			return COMMAND_HELP_FR;
		case KEY_7:
			// Take screenshot
			take_screenshot();
			break;
		case KEY_EXIT:
		case KEY_SLIM_EXIT:
			// Exit options menu
			return COMMAND_EXIT;
		}
	}
}

void kbd_help_input(void)
{
	while (1) {
		switch (getkey_opt(0xDC, NULL).key) {
		case KEY_7:
			// Take screenshot
			take_screenshot();
			break;
		case KEY_EXIT:
		case KEY_SLIM_EXIT:
			// Exit help
			return;
		}
	}
}
