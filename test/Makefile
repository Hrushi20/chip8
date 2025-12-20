test: chip8.o test.o
	cc -g -O0 test.o chip8.o -o test

chip8.o: ../chip8.c
	cc -g -O0 -c ../chip8.c -o chip8.o

test.o: test.c
	cc -g -O0 -c test.c -o test.o

clean:
	rm chip8.o test.o test
