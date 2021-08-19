CFLAGS := -O2
LIBS = -lX11

all: work/main.o work/help.o build/xgame

work/main.o: src/main.c src/help.h
	$(CC) src/main.c $(CFLAGS) -c -o work/main.o

work/help.o: src/help.c src/help.h
	$(CC) src/help.c $(CFLAGS) -c -o work/help.o

build/xgame: work/*.o
	$(CC) work/*.o $(LIBS) -o build/xgame
