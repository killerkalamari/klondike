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
			// Show options menu
			return COMMAND_OPTIONS_MENU;
		case KEY_F1:
			// Turn card(s) to waste
			return COMMAND_FLIP_TO_WASTE;
		case KEY_F2:
			// From waste
			move_from_waste();
			return COMMAND_NONE;
		case KEY_F5:
			// Automatic foundations
			return COMMAND_AUTO_FOUNDATIONS;
		case KEY_F6:
			// To foundations
			if (move_from_is_set()) {
				move_to_foundations();
				return COMMAND_MOVE;
			}
			break;
		case KEY_LEFT:
			// Move cursor to previous valid tableau pile
			move_previous_valid_pile();
			return COMMAND_NONE;
		case KEY_RIGHT:
			// Move cursor to next valid tableau pile
			move_next_valid_pile();
			return COMMAND_NONE;
		case KEY_SHIFT:
		case KEY_EXE:
			// From/to tableau pile at cursor
			if (!move_pile_is_valid())
				break;

			if (move_from_is_set()) {
				move_to_pile();
				return COMMAND_MOVE;
			}

			move_from_pile();
			return COMMAND_NONE;
		case KEY_EXIT:
			// Cancel current selection, if any
			move_clear_from();
			return COMMAND_NONE;
		case KEY_7:
			// Take screenshot (instructions at top)
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
			// Start a new game
			return COMMAND_NEW_GAME;
		case KEY_F2:
			// Flip 1 card at a time to waste
			return COMMAND_FLIP_1;
		case KEY_F3:
			// Flip 3 cards at a time to waste
			return COMMAND_FLIP_3;
		case KEY_F5:
			// Display help in English
			return COMMAND_HELP_ENG;
		case KEY_F6:
			// Display help in French
			return COMMAND_HELP_FR;
		case KEY_7:
			// Take screenshot (instructions at top)
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
			// Take screenshot (instructions at top)
			usb_fxlink_screenshot(1);
			break;
		case KEY_EXIT:
			// Exit help
			return;
		}
	}
}
