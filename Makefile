all: chip8

chip8: src/chip8.o src/instructions.o src/main.o
	gcc src/chip8.o src/instructions.o src/main.o -I./raylib/include -L./raylib/lib -pthread -o chip8 -lraylib -lm -ldl

main.o: src/main.c src/main.h
	gcc -o main.o src/main.c

instructions.o: src/instructions.c src/instructions.h
	gcc -o instructions.o src/instructions.c

chip8.o: src/chip8.c src/chip8.h
	gcc -o chip8.o src/chip8.c

clean:
	rm src/*.o chip8