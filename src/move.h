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

void move_init(void);
void move_from_waste(void);
void move_from_pile(void);
int move_get_from(void);
void move_clear_from(void);
int move_from_is_set(void);
int move_is_from_waste(void);
void move_to_foundations(void);
void move_to_pile(void);
int move_get_to(void);
int move_get_pile(void);
int move_pile_is_valid(void);
void move_previous_valid_pile(void);
void move_next_valid_pile(void);
