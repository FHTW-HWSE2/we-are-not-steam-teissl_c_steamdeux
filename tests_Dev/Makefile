
CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -pedantic -Wno-unused-parameter -I. -I../business -I../data

SRC = ../business/process_games.c \
      mock_load_games.c \
      test_all.c

all: test

test: $(SRC)
	$(CC) $(CFLAGS) -o test_runner $(SRC)

clean:
	rm -f test_runner
