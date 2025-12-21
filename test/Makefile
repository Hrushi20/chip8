test: chip8.o test.o display.o
	cc -g -O0 test.o chip8.o display.o -lncurses -o test

chip8.o: ../chip8.c
	cc -DTESTING -g -O0 -c ../chip8.c -o chip8.o

test.o: test.c
	cc -g -O0 -c test.c -o test.o

display.o: ../display.c
	cc -g -O0 -c ../display.c -o display.o

clean:
	rm chip8.o test.o test display.o
