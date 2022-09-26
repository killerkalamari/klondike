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

#include <gint/gint.h>

static void syscall(const uint32_t number)
{
	asm volatile(
		"\n	mov	%[number],r0"
		"\n	mov.l	syscall_%=,r2"
		"\n	jmp	@r2"
		"\n	nop"
		"\n	.align 4"
		"\nsyscall_%=:"
		"\n	.long	0x80010070"
		: /* no output */ : [number]"r"(number)
	);
}

static void power_off(void)
{
	asm("mov	#1,r4");
	syscall(0x3F4);
}

void syscall_power_off(void)
{
	gint_world_switch(GINT_CALL(power_off));
}
