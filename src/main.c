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
#include "help.h"
#include "kbd.h"
#include "move.h"
#include "options.h"
#include "random.h"

static int play_game(void)
{
	game_init();
	move_init();
	int continue_game = 1;
	while (continue_game) {
		display_game();
		switch(kbd_game_input()) {
		case COMMAND_OPTIONS_MENU:
			// Show options menu
			if (options() == COMMAND_NEW_GAME)
				continue_game = 0;
			break;
		case COMMAND_FLIP_TO_WASTE:
			// Flip card(s) to waste
			game_flip_to_waste();
			move_select_first();
			break;
		case COMMAND_AUTO_FOUNDATIONS:
			// Automatic foundations
			game_auto_foundations();
			move_select_first();
			break;
		case COMMAND_PREVIOUS_MOVE:
			// Select previous valid move
			move_select_previous();
			break;
		case COMMAND_NEXT_MOVE:
			// Select next valid move
			move_select_next();
			break;
		case COMMAND_MOVE:
			// Perform move
			game_perform_move(move_get_from(), move_get_to());
			move_select_first();
			break;
		case COMMAND_HELP_ENG_SLIM:
			// Display Slim help in English
			help(LANG_ENGLISH_SLIM);
			break;
		case COMMAND_TERMINATE:
			return 0;
		default:
			break;
		}
	}
	return 1;
}

int main(void)
{
	random_init();
	kbd_init();
	game_set_flip(FLIP_1);

	while (play_game());
	return 1;
}
