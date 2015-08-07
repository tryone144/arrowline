# ARROWLINE - powerline-like shell prompt generator
# (c) 2015 Bernd Busse

NAME = arrowline
VERSION = 0.7

# C Compiler
CC := $(CC) -std=gnu11

base_CFLAGS = -Wall -Wextra -pedantic -O2 -g
base_LIBS = 

pkgs = libgit2
pkgs_CFLAGS = $(shell pkg-config --cflags $(pkgs))
pkgs_LIBS = $(shell pkg-config --libs $(pkgs))

# Configuration
config_CFLAGS = -DUSE_VCS_GIT

# Compiler Flags
CFLAGS += -DPROGNAME=\"${NAME}\" -DVERSION=\"${VERSION}\"
CFLAGS := $(base_CFLAGS) $(pkgs_CFLAGS) $(config_CFLAGS) $(CFLAGS)
# Linker Flags
LDFLAGS := $(base_LIBS) $(pkgs_LIBS)

# Executable
EXE_PROMPT = arrowline
EXE_BAR = arrowbar

# Sources
SRCS = segments.c utils.c
SRCS_PROMPT = arrowline.c
SRCS_BAR = arrowbar.c

OBJS = $(SRCS:.c=.o)
OBJS_PROMPT = $(SRCS_PROMPT:.c=.o)
OBJS_BAR = $(SRCS_BAR:.c=.o)

PREFIX = /usr/local

all: $(EXE_PROMPT) $(EXE_BAR)

$(EXE_PROMPT): $(OBJS) $(OBJS_PROMPT)
	$(CC) $(OBJS) $(OBJS_PROMPT) $(LDFLAGS) -o $@

$(EXE_BAR): $(OBJS) $(OBJS_BAR)
	$(CC) $(OBJS) $(OBJS_BAR) $(LDFLAGS) -o $@

.o: $(SRCS) $(SRCS_PROMPT) $(SRCS_BAR)
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean
clean:
	rm -f *.o && rm -f $(EXE_PROMPT) && rm -f $(EXE_BAR)

install: $(EXE_PROMPT) $(EXE_BAR)
	install -Dm755 $(EXE_PROMPT) $(PREFIX)/bin/$(EXE_PROMPT)
	install -Dm755 $(EXE_BAR) $(PREFIX)/bin/$(EXE_BAR)
