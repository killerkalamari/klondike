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

#include <stdint.h>
#include <stdlib.h>
#include <gint/rtc.h>

void random_init(void)
{
	srand(rtc_ticks());
}

/*
Return the next higher power of 2, minus 1
Invalid if v is 0
*/
static uint8_t pow2_mask(uint8_t v)
{
	uint8_t m = v - 1;
	m |= m >> 1;
	m |= m >> 2;
	m |= m >> 4;
	return m;
}

uint8_t random_randint(uint8_t max)
{
	if (max == 0)
		return 0;
	uint8_t mask = pow2_mask(max);
	uint8_t v;
	do {
		v = rand() & mask;
	} while (v >= max);
	return v;
}

void random_shuffle(int arr[], uint8_t length)
{
	for (uint8_t i = 0; i < length; ++i) {
		uint8_t j = random_randint(i + 1);
		if (i != j)
			arr[i] = arr[j];
		arr[j] = i;
	}
}
