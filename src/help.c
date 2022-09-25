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
#include "kbd.h"
#include "help.h"

static const char *keys_std[] = {
	" {  } ",
	" SHIFT ",
	" F1 ",
	" F6 "
};

static const char *keys_slim[] = {
	" {  } ",
	" ^ ",
	" ~ ",
	" | "
};

static const char *desc_eng[] = {
	"Select move",
	"Move",
	"Flip card to waste",
	"Auto. foundations"
};

static const char *desc_fr[] = {
	"S$lect. d$placer",
	"D$placer",
	"Pioche: Suivant",
	"Auto. familles"
};

void help(lang_t lang)
{
	const char **keys;
	const char **desc;
	switch (lang) {
	case LANG_ENGLISH:
		keys = keys_std;
		desc = desc_eng;
		break;
	case LANG_FRENCH:
		keys = keys_std;
		desc = desc_fr;
		break;
	case LANG_ENGLISH_SLIM:
		keys = keys_slim;
		desc = desc_eng;
		break;
	case LANG_FRENCH_SLIM:
		keys = keys_slim;
		desc = desc_fr;
		break;
	default:
		return;
	}

	int count = (sizeof keys_std / sizeof (char *));
	display_help(keys, desc, count);
	kbd_help_input();
}
