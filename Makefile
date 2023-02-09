CFLAGS += -O3 -flto -march=native -pipe -std=gnu17 -Wall -Wextra

all: block

buffer.o: screen/buffer.c
window.o: screen/window.c
screen.o: screen/screen.c
timespec.o: common.c
game.o: game.c
main.o: main.c

%.o: %.c
	echo "On Windows, add -DWINDOWS to CFLAGS"
	$(CC) $(CFLAGS) -c $<

block: buffer.o window.o screen.o common.o game.o main.o
	$(CC) $(CFLAGS) $^ -lncursesw -lm -pthread -o block

clean:
	-rm *.o

distclean: clean
	-rm block

dist: distclean
	(tar zcvf ../block.tar.gz ./)

.PHONY: all clean distclean dist

VPATH = ./:./screen