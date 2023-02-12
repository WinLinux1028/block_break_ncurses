CFLAGS += -O3 -flto -march=native -pipe -std=gnu17 -Wall -Wextra

all: block

main.o: main.c
game.o: game.c
common.o: common.c
screen.o: screen/screen.c
window.o: screen/window.c
buffer.o: screen/buffer.c

%.o: %.c
	$(CC) $(CFLAGS) -c $<

block: main.o game.o common.o screen.o window.o buffer.o
	$(CC) $(CFLAGS) $^ -lncurses -lm -pthread -o block

clean:
	-rm *.o

distclean: clean
	-rm block

dist: distclean
	(tar zcvf ../block.tar.gz ./)

.PHONY: all clean distclean dist

VPATH = ./:./screen
