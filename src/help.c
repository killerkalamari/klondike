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

static const char *term[] = {
	" SHIFT ",
	" EXIT ",
	" {  } ",
	" F1 ",
	" F2 ",
	" F5 ",
	" F6 "
};

static const char *desc_eng[] = {
	"Take/Move",
	"Cancel selection",
	"Pick pile",
	"Flip card to waste",
	"From waste",
	"Auto. foundations",
	"To foundations"
};

static const char *desc_fr[] = {
	"Choisir/D$placer",
	"Annuler s$lection",
	"S$lectionner pile",
	"Pioche: Suivant",
	"Pioche: Pr$c$dent",
	"Auto. familles",
	"Poser sur famille"
};

void help(lang_t lang)
{
	const char **desc;
	switch (lang) {
	case LANG_FRENCH:
		desc = desc_fr;
		break;
	default:
		desc = desc_eng;
		break;
	}

	int count = (sizeof term / sizeof (char *));

	display_help(term, desc, count);
	kbd_help_input();
}
