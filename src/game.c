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

#include "random.h"
#include "game.h"

#define DECK_SIZE (RANKS * SUITS)
#define DOWNCARD_COUNT ((PILE_COUNT - 1) * PILE_COUNT / 2)
#define WASTE_COUNT (DECK_SIZE - DOWNCARD_COUNT - PILE_COUNT)

#define ACE 0
#define KING 12
#define CARD_INVALID -1
#define FROM_INVALID -1

struct game_state {
	flip_t flip;
	card_t foundation[SUITS];
	card_t waste[WASTE_COUNT];
	int waste_count;
	int waste_rem;
	card_t downcard[DOWNCARD_COUNT];
	int downcard_count;
	int pile_downcard_count[PILE_COUNT];
	card_t tableau[PILE_COUNT][RANKS];
	int pile_count[PILE_COUNT];
};

static struct game_state state;

void game_set_flip(flip_t flip)
{
	if (flip == FLIP_1 && state.flip != FLIP_1) {
		state.waste_rem = 1;
		if (state.waste_rem > state.waste_count)
			state.waste_rem = state.waste_count;
	} else if (flip == FLIP_3 && state.flip != FLIP_3) {
		state.waste_rem = 3;
		if (state.waste_rem > state.waste_count)
			state.waste_rem = state.waste_count;
	}
	state.flip = flip;
}

static void set_card(card_t *card, int card_number)
{
	card->rank = card_number % RANKS;
	card->suit = card_number / RANKS;
	card->color = card->suit >> 1;
}

void game_init(void)
{
	// Shuffle deck
	int deck[DECK_SIZE];
	random_shuffle(deck, DECK_SIZE);

	// Init foundations
	for (int i = 0; i < SUITS; ++i)
		set_card(&(state.foundation[i]), CARD_INVALID);

	// Init waste
	for (int i = 0; i < WASTE_COUNT; ++i)
		set_card(&(state.waste[i]), deck[i]);
	state.waste_count = WASTE_COUNT;
	state.waste_rem = (state.flip == FLIP_1) ? 1 : 3;

	// Init downcards
	for (int i = 0; i < DOWNCARD_COUNT; ++i)
		set_card(&(state.downcard[i]), deck[i + WASTE_COUNT]);
	state.downcard_count = DOWNCARD_COUNT;

	// Init tableau
	for (int i = 0; i < PILE_COUNT; ++i) {
		state.pile_downcard_count[i] = i;
		set_card(&(state.tableau[i][0]),
				deck[i + WASTE_COUNT + DOWNCARD_COUNT]);
		state.pile_count[i] = 1;
	}
}

int game_talon_is_empty(void)
{
	return (state.waste_count - state.waste_rem) <= 0;
}

int game_waste_remaining(void)
{
	return state.waste_rem;
}

int game_pile_downcard_count(int pile_index)
{
	return state.pile_downcard_count[pile_index];
}

int game_pile_count(int pile_index)
{
	return state.pile_count[pile_index];
}

card_t *game_get_waste_card(int index)
{
	return &(state.waste[index]);
}

card_t *game_get_foundation_card(int suit)
{
	return &(state.foundation[suit]);
}

card_t *game_get_pile_top_card(int pile_index)
{
	return &(state.tableau[pile_index][0]);
}

card_t *game_get_pile_bottom_card(int pile_index)
{
	return &(state.tableau[pile_index][game_pile_count(pile_index) - 1]);
}

int game_card_is_valid(card_t *card)
{
	return card && card->rank >= 0;
}

void game_flip_to_waste(void)
{
	state.waste_rem = (state.flip == FLIP_1) ? 1 : 3;
	if (state.waste_rem > state.waste_count)
		state.waste_rem = state.waste_count;

	for (int j = 0; j < state.waste_rem; ++j) {
		// Move card from back to front
		card_t card = state.waste[state.waste_count - 1];
		for (int i = state.waste_count - 2; i >= 0; --i)
			state.waste[i + 1] = state.waste[i];
		state.waste[0] = card;
	}
}

static int to_foundations_is_valid(card_t from_card)
{
	int suit = from_card.suit;
	card_t foundation_card = state.foundation[suit];
	if (!game_card_is_valid(&foundation_card))
		return from_card.rank == ACE;
	return from_card.rank == (foundation_card.rank + 1);
}

static int to_pile_is_valid(card_t from_card, int from_i, int to_i)
{
	int pile_count = state.pile_count[to_i];
	if (pile_count < 1) {
		if (from_card.rank != KING)
			return 0;
		if (from_i == FROM_INVALID)
			return 1;
		return state.pile_downcard_count[from_i] > 0;
	}
	card_t to_card = state.tableau[to_i][pile_count - 1];
	return to_card.color != from_card.color &&
		from_card.rank == (to_card.rank - 1);
}

static move_t single_validate_move(int from, int to, int *suit)
{
	card_t card;
	int from_i;
	if (from == WASTE) {
		from_i = FROM_INVALID;
		if (state.waste_count < 1)
			return MOVE_INVALID;
		card = state.waste[0];
	} else {
		from_i = from - 1;
		int pile_count = state.pile_count[from_i];
		if (pile_count < 1)
			return MOVE_INVALID;
		card = state.tableau[from_i][pile_count - 1];
	}

	if (to == FOUNDATIONS) {
		if (suit)
			*suit = card.suit;
		return to_foundations_is_valid(card) ? MOVE_SINGLE :
			MOVE_INVALID;
	}

	return to_pile_is_valid(card, from_i, to - 1) ? MOVE_SINGLE :
							MOVE_INVALID;
}

static int pile_move_is_valid(int from, int to)
{
	if (from == WASTE || to == FOUNDATIONS)
		return 0;
	int from_i = from - 1;
	if (state.pile_count[from_i] < 1)
		return 0;
	card_t card = state.tableau[from_i][0];
	return to_pile_is_valid(card, from_i, to - 1);
}

move_t game_validate_move(int from, int to, int *suit)
{
	if (to == from || from < WASTE || from >= FOUNDATIONS || to <= WASTE ||
			to > FOUNDATIONS)
		return MOVE_INVALID;

	move_t move = single_validate_move(from, to, suit);
	if (move != MOVE_INVALID)
		return move;

	return pile_move_is_valid(from, to) ? MOVE_PILE : MOVE_INVALID;
}

int game_perform_move(int from, int to)
{
	card_t card;
	int *from_pile_count = 0;
	int *to_pile_count;
	int from_i;
	int to_i;

	move_t move = game_validate_move(from, to, 0);
	switch (move) {
	case MOVE_INVALID:
		return 1;
	case MOVE_SINGLE:
		// Remove FROM card
		if (from == WASTE) {
			card = state.waste[0];
			for (int i = 1; i < state.waste_count; ++i)
				state.waste[i - 1] = state.waste[i];
			--(state.waste_count);
			if (state.waste_rem > 1 || state.waste_count == 0)
				--(state.waste_rem);
		} else {
			from_i = from - 1;
			from_pile_count = state.pile_count + from_i;
			--*from_pile_count;
			card = state.tableau[from_i][*from_pile_count];
		}

		if (to == FOUNDATIONS) {
			// Set foundation pile
			state.foundation[card.suit] = card;
		} else {
			// Append 1 card to TO pile
			to_i = to - 1;
			to_pile_count = state.pile_count + to_i;
			state.tableau[to_i][*to_pile_count] = card;
			++*to_pile_count;
		}
		break;
	case MOVE_PILE:
		// Append entire FROM pile to TO pile
		from_i = from - 1;
		to_i = to - 1;
		from_pile_count = state.pile_count + from_i;
		to_pile_count = state.pile_count + to_i;
		for (int i = 0; i < *from_pile_count; ++i) {
			card = state.tableau[from_i][i];
			state.tableau[to_i][*to_pile_count] = card;
			++*to_pile_count;
		}

		// Remove all cards from FROM pile
		*from_pile_count = 0;
		break;
	}

	// If FROM pile is now empty: flip a downcard, if any remain
	if (from_pile_count && *from_pile_count == 0) {
		from_i = from - 1;
		int *pile_downcard_count = state.pile_downcard_count + from_i;
		if (*pile_downcard_count > 0) {
			--*pile_downcard_count;
			card = state.downcard[state.downcard_count - 1];
			--(state.downcard_count);
			state.tableau[from_i][0] = card;
			*from_pile_count = 1;
		}
	}
	return 0;
}

void game_auto_foundations(void)
{
	int success;
	do {
		success = 0;
		for (int from = WASTE; from < FOUNDATIONS; ++from)
			if (game_perform_move(from, FOUNDATIONS) == 0)
					success = 1;
	} while(success);
}
