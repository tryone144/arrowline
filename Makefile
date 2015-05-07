# C Compiler
CC = gcc

# Compiler Flags
CFLAGS = -Wall -std=gnu11
# Linker Flags
LDFLAGS =

SRCS = arrowline.c utils.c segments.c
OBJS = arrowline.o utils.o segments.o
# Executable
EXE = arrowline

PREFIX = /usr/local

all: $(EXE)

$(EXE): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $@

.o: $(SRCS)
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm *.o && rm $(EXE)

install: $(EXE)
	install -Dm755 $(EXE) $(PREFIX)/bin/$(EXE)
