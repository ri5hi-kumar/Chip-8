all:
	gcc -I./raylib/include -L./raylib/lib -pthread -o chip8 ./src/main.c -lraylib -lm -ldl

clean:
	rm chip8