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

#include "display.h"
#include "game.h"
#include "kbd.h"
#include "help.h"

command_t options(void)
{
	display_options();
	switch (kbd_options_input()) {
	case COMMAND_NEW_GAME:
		return COMMAND_NEW_GAME;
	case COMMAND_FLIP_1:
		game_set_flip(FLIP_1);
		break;
	case COMMAND_FLIP_3:
		game_set_flip(FLIP_3);
		break;
	case COMMAND_HELP_ENG:
		help(LANG_ENGLISH);
		break;
	case COMMAND_HELP_FR:
		help(LANG_FRENCH);
		break;
	default:
		break;
	}
	return COMMAND_NONE;
}
