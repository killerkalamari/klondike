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

#include "game.h"
#include "move.h"

#define INVALID_FROM -1
#define DIR_LEFT -1
#define DIR_RIGHT 1

struct move_state {
	int from;
	int to;
	int pile;
};

static struct move_state state;

static int valid_from(int from)
{
	for (int i = 1; i <= FOUNDATIONS; ++i)
		if (game_validate_move(from, i) != MOVE_INVALID)
			return 1;
	return 0;
}

static void find_valid_pile(int dir, int allow_same_rank)
{
	int pile = state.pile;
	for (int i = 0; i <= PILE_COUNT; ++i) {
		pile += dir;
		if (pile <= WASTE)
			pile = PILE_COUNT;
		if (pile > PILE_COUNT)
			pile = LEFT_PILE;
		if (move_from_is_set()) {
			move_t move = game_validate_move(state.from, pile);
			if (move != MOVE_INVALID && (
					move != MOVE_SINGLE_SAME_RANK ||
					allow_same_rank)) {
				state.pile = pile;
				return;
			}
		} else if (valid_from(pile)) {
			state.pile = pile;
			return;
		}
	}
	state.pile = 0;
}

static void pick_pile(void)
{
	state.pile = 0;
	find_valid_pile(DIR_RIGHT, 0);
	if (state.pile == 0 && !move_from_is_set() && valid_from(WASTE)) {
		state.from = WASTE;
		find_valid_pile(DIR_RIGHT, 0);
		return;
	}
	find_valid_pile(DIR_RIGHT, 1);
}

void move_init(void)
{
	move_clear_from();
	pick_pile();
}

void move_from_waste(void)
{
	if (valid_from(WASTE)) {
		state.from = WASTE;
		pick_pile();
	} else {
		move_clear_from();
	}
}

void move_from_pile(void)
{
	state.from = state.pile;
	pick_pile();
}

int move_get_from(void)
{
	return state.from;
}

void move_clear_from(void)
{
	state.from = INVALID_FROM;
	pick_pile();
}

int move_from_is_set(void)
{
	return state.from >= WASTE;
}

int move_is_from_waste(void)
{
	return state.from == WASTE;
}

void move_to_foundations(void)
{
	state.to = FOUNDATIONS;
}

void move_to_pile(void)
{
	state.to = state.pile;
}

int move_get_to(void)
{
	return state.to;
}

int move_get_pile(void)
{
	return state.pile;
}

int move_pile_is_valid(void)
{
	return state.pile >= 1 && state.pile <= PILE_COUNT;
}

void move_previous_valid_pile(void)
{
	find_valid_pile(DIR_LEFT, 1);
}

void move_next_valid_pile(void)
{
	find_valid_pile(DIR_RIGHT, 1);
}
