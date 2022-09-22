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

#pragma once

typedef enum {
	COMMAND_NONE,
	COMMAND_FLIP_TO_WASTE,
	COMMAND_AUTO_FOUNDATIONS,
	COMMAND_PREVIOUS_MOVE,
	COMMAND_NEXT_MOVE,
	COMMAND_MOVE,
	COMMAND_OPTIONS_MENU,
	COMMAND_NEW_GAME,
	COMMAND_HELP_ENG,
	COMMAND_HELP_FR,
	COMMAND_FLIP_1,
	COMMAND_FLIP_3,
	COMMAND_EXIT
} command_t;

void kbd_init(void);
command_t kbd_game_input(void);
command_t kbd_options_input(void);
void kbd_help_input(void);
