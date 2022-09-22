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

#define INVALID_LOCATION -1
#define DIR_LEFT -1
#define DIR_RIGHT 1

struct move_state {
	int from;
	int to;
	int is_entire_pile;
	int suit;
};

static struct move_state state;

void move_init(void)
{
	move_select_first();
}

int move_get_from(void)
{
	return state.from;
}

int move_get_to(void)
{
	return state.to;
}

int move_is_from_waste(void)
{
	return state.from == WASTE;
}

int move_is_to_foundations(int suit)
{
	return state.to == FOUNDATIONS && state.suit == suit;
}

int move_is_entire_pile(void)
{
	return state.is_entire_pile;
}

static void adjust_from(int dir)
{
	state.from += dir;
	if (state.from < WASTE)
		state.from = RIGHTMOST_PILE;
	if (state.from >= FOUNDATIONS)
		state.from = WASTE;
}

static void adjust_to(int dir)
{
	state.to += dir;
	if (state.to <= WASTE) {
		state.to = FOUNDATIONS;
		adjust_from(dir);
	}
	if (state.to > FOUNDATIONS) {
		state.to = LEFTMOST_PILE;
		adjust_from(dir);
	}
}

static void find_valid_move(int dir)
{
	if (state.from == INVALID_LOCATION || state.to == INVALID_LOCATION)
		return;

	int old_from = state.from;
	int old_to = state.to;
	do {
		int suit = 0;
		move_t move = game_validate_move(state.from, state.to, &suit);
		if (move != MOVE_INVALID) {
			state.is_entire_pile = (move == MOVE_PILE);
			state.suit = suit;
			return;
		}
		adjust_to(dir);
	} while (state.from != old_from || state.to != old_to);

	state.from = INVALID_LOCATION;
	state.to = INVALID_LOCATION;
}

void move_select_first(void)
{
	state.from = WASTE;
	state.to = LEFTMOST_PILE;
	find_valid_move(DIR_RIGHT);
}

void move_select_previous(void)
{
	adjust_to(DIR_LEFT);
	find_valid_move(DIR_LEFT);
}

void move_select_next(void)
{
	adjust_to(DIR_RIGHT);
	find_valid_move(DIR_RIGHT);
}
