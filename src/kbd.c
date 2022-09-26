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
#include <gint/hardware.h>
#include "syscall.h"
#include "kbd.h"

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
static uint kbd_getkey(void)
{
	uint key = getkey_opt(GETKEY_BACKLIGHT | GETKEY_MENU, NULL).key;
	if (key == KEY_7) {
		// Take screenshot
		if (usb_is_open())
			usb_fxlink_screenshot(1);
		return 0;
	}
	return key;
}

command_t kbd_game_input(void)
{
	while (1) {
		uint key = kbd_getkey();
		if (key == KEY_ACON) {
			syscall_power_off();
			return COMMAND_NONE;
		}
		if (isSlim())
			switch (key) {
			case KEY_OPTN:
				return COMMAND_OPTIONS_MENU;
			case KEY_DOWN:
				return COMMAND_FLIP_TO_WASTE;
			case KEY_UP:
				return COMMAND_AUTO_FOUNDATIONS;
			case KEY_LEFT:
				return COMMAND_PREVIOUS_MOVE;
			case KEY_RIGHT:
				return COMMAND_NEXT_MOVE;
			case KEY_POWER:
			case KEY_EXE:
				return COMMAND_MOVE;
			case KEY_HELP:
				return COMMAND_HELP_ENG_SLIM;
			}
		else
			switch (key) {
			case KEY_OPTN:
				return COMMAND_OPTIONS_MENU;
			case KEY_F1:
				return COMMAND_FLIP_TO_WASTE;
			case KEY_F6:
				return COMMAND_AUTO_FOUNDATIONS;
			case KEY_LEFT:
				return COMMAND_PREVIOUS_MOVE;
			case KEY_RIGHT:
				return COMMAND_NEXT_MOVE;
			case KEY_SHIFT:
			case KEY_EXE:
				return COMMAND_MOVE;
			}
	}
}

command_t kbd_options_input(void)
{
	while (1) {
		uint key = kbd_getkey();
		if (isSlim())
			switch (key) {
			case KEY_F1:
				return COMMAND_NEW_GAME;
			case KEY_F2:
				return COMMAND_FLIP_1;
			case KEY_F3:
				return COMMAND_FLIP_3;
			case KEY_F5:
				return COMMAND_HELP_ENG_SLIM;
			case KEY_F6:
				return COMMAND_HELP_FR_SLIM;
			case KEY_EXIT:
				// Exit options menu
				return COMMAND_EXIT;
			}
		else
			switch (key) {
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
			case KEY_EXIT:
				// Exit options menu
				return COMMAND_EXIT;
			}
	}
}

void kbd_help_input(void)
{
	while (kbd_getkey() != KEY_EXIT);
}
