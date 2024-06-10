CC=gcc
RAYLIB_FLAGS=-lraylib -lm -ldl
RAYLIB_LIBS=-I./raylib/include -L./raylib/lib -pthread

EXECUTABLE=chip8

SOURCEDIR=src/

HEADER_FILES=instructions.h chip8.h chip8_context.h
SOURCE_FILES=main.c chip8.c instructions.c

# The addprefix function in Makefile takes a prefix and a list of names, and it prepends the prefix to each name in the list.
HEADERS_FP=$(addprefix $(SOURCEDIR),$(HEADER_FILES))
SOURCE_FP=$(addprefix $(SOURCEDIR),$(SOURCE_FILES))

# In Makefiles, variable substitution allows you to create new strings based on the contents of existing variables.
OBJECTS=$(SOURCE_FP:.c = .o)

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(RAYLIB_LIBS) -o $(EXECUTABLE) $(RAYLIB_FLAGS)

%.o: %.c $(HEADERS_FP)
	$(CC) $(CFLAGS) -o $@ $< 

clean:
	rm -rf src/*.o $(EXECUTABLE)