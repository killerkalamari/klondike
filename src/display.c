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

#include <gint/display.h>
#include "game.h"
#include "move.h"

extern bopti_image_t img_card;
extern bopti_image_t img_card_back;
extern bopti_image_t img_card_gray;
extern bopti_image_t img_empty;
extern bopti_image_t img_options;
extern bopti_image_t img_rank;
extern bopti_image_t img_suit;
extern font_t font_help;

static int to_x(int column)
{
	return 18 * column + 1;
}

static void draw_card_x(int x, int y, card_t *card, int invert, int gray)
{
	if (!game_card_is_valid(card))
		return;

	if (gray)
		dimage(x, y, &img_card_gray);
	else
		dimage(x, y, &img_card);
	dsubimage(x + 2, y + 2, &img_rank, 8 * card->rank, 0, 8, 7,
			DIMAGE_NOCLIP);
	dsubimage(x + 5, y + 10, &img_suit, 7 * card->suit, 0, 7, 8,
			DIMAGE_NOCLIP);

	if (invert)
		drect(x + 1, y + 1, x + 15, y + 18, C_INVERT);
}

static void draw_card(int column, int y, card_t *card, int invert, int gray)
{
	draw_card_x(to_x(column), y, card, invert, gray);
}

static void draw_downcards(int column, int count)
{
	int x = to_x(column);
	dsubimage(x, 21, &img_card, 0, 0, 17, 2, DIMAGE_NOCLIP);
	if (count > 1)
		for (int i = 0; i < count; ++i)
			dpixel(x + 14 - 2 * i, 22, C_BLACK);
}

void display_game(void)
{
	// Clear VRAM
	dclear(C_WHITE);

	// Talon
	if (!game_talon_is_empty())
		dimage(1, 0, &img_card_back);

	// Waste
	int gray = move_is_from_waste();
	int waste_remaining = game_waste_remaining();
	for (int i = waste_remaining - 1; i >= 0; --i) {
		draw_card_x(
			9 * (waste_remaining - i) + 10,
			0,
			game_get_waste_card(i),
			0,
			gray && (i == 0)
		);
	}

	// Foundations
	for (int i = 0; i < SUITS; ++i)
		draw_card(i + 3, 0, game_get_foundation_card(i), 0, 0);

	// Tableau piles
	int from_selected = move_from_is_set();
	for (int i = 0; i < PILE_COUNT; ++i) {
		int pile_downcard = game_pile_downcard_count(i);
		if (pile_downcard)
			draw_downcards(i, pile_downcard);
		int pile_count = game_pile_count(i);
		int invert = (move_get_pile() == (i + 1));
		int gray = (from_selected && move_get_from() == (i + 1));
		if (pile_count)
			draw_card(i, 23, game_get_pile_top_card(i), invert,
					gray);
		else if (invert)
			dimage(to_x(i), 23, &img_empty);
		if (pile_count > 1)
			draw_card(i, 44, game_get_pile_bottom_card(i), invert,
					gray);
	}

	// Draw VRAM to display
	dupdate();
}

void display_options(void)
{
	dimage(0, 55, &img_options);

	// Draw VRAM to display
	dupdate();
}

void display_help(const char *term[], const char *desc[], int count)
{
	dfont(&font_help);
	dclear(C_WHITE);
	drect(0, 0, 28, 58, C_BLACK);
	for (int i = 0; i < count; ++i) {
		int y = 8 * i + 2;
		dtext_opt(0, y, C_WHITE, C_BLACK, DTEXT_LEFT, DTEXT_TOP,
				term[i]);
		dtext(31, y, C_BLACK, desc[i]);
	}
	dupdate();
}
