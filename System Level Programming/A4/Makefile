PROJECT_ROOT = $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

CC :=gcc
CFLAGS := -Werror -Wall
LDFLAGS := -lrt -lpthread -g
OBJS  := User MagicConchShell

all: User MagicConchShell

User: User.c
	$(CC) $(CFLAGS) User.c util.c -o User $(LDFLAGS) -DSTART=startUser

MagicConchShell: MagicConchShell.c
	$(CC) $(CFLAGS) MagicConchShell.c util.c -o MagicConchShell $(LDFLAGS) -DSTART=startMagicConchShell

run:
	./User

clean:
	rm -fr ASS4 $(OBJS)