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

#define RANKS 13
#define SUITS 4
#define PILE_COUNT 7

#define WASTE 0
#define LEFT_PILE 1
#define FOUNDATIONS 8

typedef enum {
	FLIP_1,
	FLIP_3
} flip_t;

typedef enum {
	MOVE_INVALID,
	MOVE_SINGLE,
	MOVE_SINGLE_SAME_RANK,
	MOVE_PILE
} move_t;

typedef struct {
	int rank;
	int suit;
	int color;
} card_t;

void game_set_flip(flip_t flip);
void game_init(void);
int game_talon_is_empty(void);
int game_waste_remaining(void);
int game_pile_downcard_count(int pile_index);
int game_pile_count(int pile_index);
card_t *game_get_waste_card(int index);
card_t *game_get_foundation_card(int suit);
card_t *game_get_pile_top_card(int pile_index);
card_t *game_get_pile_bottom_card(int pile_index);
int game_card_is_valid(card_t *card);
void game_flip_to_waste(void);
move_t game_validate_move(int from, int to);
int game_perform_move(int from, int to);
void game_auto_foundations(void);
