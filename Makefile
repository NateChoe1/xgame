CFLAGS := -O2 -march=native
LIBS = -lX11

SRC = $(wildcard src/*.c)
OBJ = $(shell echo $(SRC) | sed "s/\.c/.o/g" | sed "s/src/work/g")
#There's probably a better way to do this, but I'm no Makefile expert.

all: $(OBJ) build/xgame

work/%.o: src/%.c
	$(CC) $< $(CFLAGS) -c -o $@

build/xgame: $(wildcard work/*.o)
	$(CC) work/*.o $(LIBS) -o build/xgame

clean: work/*.o
	rm work/*.o
