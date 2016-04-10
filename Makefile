# ARROWLINE - powerline-like shell prompt generator
# (c) 2016 Bernd Busse

NAME = arrowline
VERSION = 0.7.5

# C Compiler
CC := $(CC) -std=gnu11

base_CFLAGS = -Wall -Wextra -pedantic -O2 -g
base_LIBS = 

#pkgs = 
#pkgs_CFLAGS = $(shell pkg-config --cflags $(pkgs))
#pkgs_LIBS = $(shell pkg-config --libs $(pkgs))

# Configuration
config_CFLAGS = -DUSE_VCS_GIT

# Compiler Flags
CFLAGS += -DPROGNAME=\"${NAME}\" -DVERSION=\"${VERSION}\"
CFLAGS := $(base_CFLAGS) $(pkgs_CFLAGS) $(config_CFLAGS) $(CFLAGS)
# Linker Flags
LDFLAGS := $(base_LIBS) $(pkgs_LIBS)

# Executable
EXE = arrowline

# Sources
SRCS = arrowline.c renderer.c segments.c utils.c
OBJS = $(SRCS:.c=.o)

PREFIX = /usr/local

all: $(EXE)

$(EXE): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $@

.o: $(SRCS)
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean
clean:
	rm -f *.o && rm -f $(EXE)

install: $(EXE)
	install -Dm755 $(EXE) $(PREFIX)/bin/$(EXE)
