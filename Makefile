chip8: chip8.o display.o
	cc -g -O0 chip8.o display.o -lncurses -o chip8

chip8.o: chip8.c
	cc -g -O0 -c chip8.c -o chip8.o

display.o: display.c
	cc -g -O0 -c display.c -o display.o

clean:
	rm chip8.o chip8 display.o
