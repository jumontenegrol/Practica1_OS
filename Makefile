# Makefile para el proyecto

CC = gcc
CFLAGS = -Wall -Wextra -O2
PKG = pkg-config
GTK_CFLAGS = $(shell $(PKG) --cflags gtk+-3.0)
GTK_LIBS = $(shell $(PKG) --libs gtk+-3.0)

# --- Build-only targets ---
build: build-indexer build-back build-gui

build-indexer:
	$(CC) $(CFLAGS) indexer.c hashmap.c -o indexer

build-back:
	$(CC) $(CFLAGS) back.c hashmap.c -o back

build-gui:
	$(CC) $(CFLAGS) $(GTK_CFLAGS) gui.c -o gui $(GTK_LIBS)

# --- Run targets ---
# make indexer -> build indexer (if needed) then run it
indexer: build-indexer
	./indexer

# start backend in background
run-back: build-back
	./back &

# run gui in foreground
run-gui: build-gui
	./gui

# run: start backend (bg) then gui (fg)
run: run-back run-gui

# make all: build everything, then run indexer, start backend (bg) and gui (fg)
all: build
	./indexer
	./back & \
	sleep 1; \
	./gui

clean:
	rm -f gui back indexer header.dat index.dat

.PHONY: build build-indexer build-back build-gui indexer run-back run-gui run all clean
