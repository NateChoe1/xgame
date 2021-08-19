CFLAGS := -O2 -march=native
LIBS = -lX11

all: work/main.o work/help.o work/level1.o build/xgame

work/main.o: src/main.c src/help.h
	$(CC) src/main.c $(CFLAGS) -c -o work/main.o

work/help.o: src/help.c src/help.h
	$(CC) src/help.c $(CFLAGS) -c -o work/help.o

work/level1.o: src/level1.c src/help.h
	$(CC) src/level1.c $(CFLAGS) -c -o work/level1.o

build/xgame: work/*.o
	$(CC) work/*.o $(LIBS) -o build/xgame

clean: work/*.o
	rm work/*.o
