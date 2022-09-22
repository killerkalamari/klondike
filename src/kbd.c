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

/*
How to take an in-game screenshot:
1. Run: fxlink -iw
2. Connect the calculator via USB then press EXIT
	(don't press F1 - F3)
3. Press "7" in the game
*/

#include <gint/keyboard.h>
#include <gint/usb-ff-bulk.h>
#include "move.h"
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

void kbd_init(void)
{
	usb_interface_t const *interfaces[] = { &usb_ff_bulk, NULL };
	usb_open(interfaces, GINT_CALL_NULL);
}

command_t kbd_game_input(void)
{
	while (1) {
		switch (getkey_opt(0xCC, NULL).key) {
		case KEY_OPTN:
			return COMMAND_OPTIONS_MENU;
		case KEY_F1:
			return COMMAND_FLIP_TO_WASTE;
		case KEY_F2:
			move_from_waste();
			return COMMAND_NONE;
		case KEY_F5:
			return COMMAND_AUTO_FOUNDATIONS;
		case KEY_F6:
			if (move_from_is_set()) {
				move_to_foundations();
				return COMMAND_MOVE;
			}
			break;
		case KEY_LEFT:
			move_previous_valid_pile();
			return COMMAND_NONE;
		case KEY_RIGHT:
			move_next_valid_pile();
			return COMMAND_NONE;
		case KEY_SHIFT:
		case KEY_EXE:
			if (!move_pile_is_valid())
				break;

			if (move_from_is_set()) {
				move_to_pile();
				return COMMAND_MOVE;
			}

			move_from_pile();
			return COMMAND_NONE;
		case KEY_EXIT:
			move_clear_from();
			return COMMAND_NONE;
		case KEY_7:
			// Take screenshot
			usb_fxlink_screenshot(1);
			break;
		}
	}
}

command_t kbd_options_input(void)
{
	while (1) {
		switch (getkey_opt(0xCC, NULL).key) {
		case KEY_F1:
			return COMMAND_NEW_GAME;
		case KEY_F2:
			return COMMAND_FLIP_1;
		case KEY_F3:
			return COMMAND_FLIP_3;
		case KEY_F5:
			return COMMAND_HELP_ENG;
		case KEY_F6:
			return COMMAND_HELP_FR;
		case KEY_7:
			// Take screenshot
			usb_fxlink_screenshot(1);
			break;
		case KEY_EXIT:
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
			usb_fxlink_screenshot(1);
			break;
		case KEY_EXIT:
			// Exit help
			return;
		}
	}
}
