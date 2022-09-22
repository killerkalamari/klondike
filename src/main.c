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

static void play_game(void)
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
			move_from_waste();
			break;
		case COMMAND_MOVE:
			game_perform_move(move_get_from(), move_get_to());
			move_clear_from();
			break;
		case COMMAND_AUTO_FOUNDATIONS:
			// Automatic foundations
			game_auto_foundations();
			move_clear_from();
			// Select previous valid move
			// Select next valid move
			// Perform move
			break;
		default:
			break;
		}
	}
}

int main(void)
{
	random_init();
	kbd_init();
	game_set_flip(FLIP_1);

	while (1)
		play_game();
}
