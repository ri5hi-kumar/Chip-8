# Chip-8 in C
CHIP-8 is an interpreted programming language, developed by Joseph Weisbecker on his 1802 microprocessor. It was initially used on the COSMAC VIP and Telmac 1800, which were 8-bit microcomputers made in the mid-1970s.

This is my implementation of the Chip-8 interperator in C using [Raylib](https://www.raylib.com/) for graphics.

# To install Raylib
To build your raylib game for GNU/Linux you need to download raylib git repository. raylib already comes with ready-to-use makefiles and CMake system to compile source code, examples and templates.
Follow [this](https://github.com/raysan5/raylib/wiki/Working-on-GNU-Linux) link for more information.

# Running
```
$ make
$ ./chip8 ./roms/<name/of/file>
```

# Keyboard Layout:

## Chip8 Keypad:
|   |   |   |   |
|---|---|---|---|
| 1 | 2 | 3 | C |
| 4 | 5 | 6 | D |
| 7 | 8 | 9 | E |
| A | 0 | B | F |

## Emulator Keyboard Mapping:
|   |   |   |   |
|---|---|---|---|
| 1 | 2 | 3 | 4 |
| Q | W | E | R |
| A | S | D | F |
| Z | X | C | V |

# Resources
- [CHIP-8 on Wikipedia](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#2.4)
- [Guide to making a CHIP-8 emulator](https://tobiasvl.github.io/blog/write-a-chip-8-emulator/)
- [How to write an emulator (CHIP-8 interpreter)](https://multigesture.net/articles/how-to-write-an-emulator-chip-8-interpreter/)