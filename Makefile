# Klondike Solitaire for Casio fx-9750GIII
# Copyright (C) 2022  Jeffry Johnston
#
# This file is part of Klondike Solitaire.
#
# Klondike Solitaire is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Klondike Solitaire is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Klondike Solitaire.  If not, see <https://www.gnu.org/licenses/>.

name := Klondike
version := 1.4

headers :=		\
	src/display.h	\
	src/game.h	\
	src/help.h	\
	src/kbd.h	\
	src/move.h	\
	src/random.h	\

srcs :=			\
	display.c	\
	game.c		\
	help.c		\
	kbd.c		\
	main.c		\
	move.c		\
	options.c	\
	random.c	\

images :=		\
	card_back.png	\
	card_gray.png	\
	card.png	\
	font_help.png	\
	options.png	\
	rank.png	\
	suit.png

icon := assets/icon.png

CC := sh-elf-gcc
CFLAGS := -DFX9860G -DTARGET_FX9860G -m3 -mb -ffreestanding -nostdlib \
		-Wa,--dsp -Wall -Wextra -Os -fstrict-volatile-bitfields
LDFLAGS := -nostdlib -Wl,--no-warn-rwx-segments -T fx9860g.ld

libs := $(shell $(CC) -print-file-name=libgint-fx.a) \
	$(shell $(CC) -print-file-name=libc.a) -lopenlibm -lgcc
objs := $(srcs:%=build/%.o) $(images:%=build/%.o)
elf := build/$(name).elf
bin := build/$(name).bin
filename := $(name).g1a
add_in := build/$(filename)

.PHONY: all
all: $(add_in)

$(add_in): $(bin) $(icon) Makefile
	fxgxa --g1a -n $(name) -i $(icon) --version="$(version)" -o $@ $<

$(bin): $(elf)
	sh-elf-objcopy -O binary -R .bss -R .gint_bss $< $@

$(elf): $(objs)
	$(CC) $(LDFLAGS) -o $@ $^ $(libs) $(libs)

build/%.c.o: src/%.c $(headers)
	$(CC) $(CFLAGS) -c -o $@ $<

build/%.png.o: assets/%.png
	fxconv --toolchain=sh-elf --fx -o $@ $<

$(shell mkdir -p build)

.PHONY: install
install: $(add_in)
	@while true; do \
	    DEVICE=`blkid | grep LABEL_FATBOOT=\"CASIO\" | cut -d':' -f1`; \
	    [ -n "$$DEVICE" ] && break; \
	    sleep 0.25; \
	done; \
	while true; do \
	    DEST=`findmnt -nr -o target $$DEVICE`; \
	    [ -n "$$DEST" ] && break; \
	    sleep 0.25; \
	done; \
	rm -f "$$DEST/"$(filename); \
	cp -v $(add_in) "$$DEST"/; \
	eject "$$DEST" 2>/dev/null || true

.PHONY: clean
clean:
	$(RM) -r build/
