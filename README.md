# Asteroids

My take on the original arcade game. Made with [raylib](https://www.raylib.com/). 

[Play it Here]()

## Controls

##### Left:         `rotate left`
##### Right:        `rotate right`
##### Up:           `thrust`
##### Left CTRL:    `shoot`
##### Space:        `hyperspace`
##### TAB:          `toggle fullscreen`

## Building
To build the game, ensure you have a C compiler (e.g., `gcc` or `clang`) and raylib installed. Compile asteroids.c with:
`gcc asteroids.c -o asteroids -lraylib -lgdi32 -lwinmm`

Optional flags: `-Wall -std=c99` for warnings and C99 standard compliance.

### Troubleshooting: 
If you encounter errors like `error: incompatible types when assigning to type 'Vector2' from type 'int'` or `raymath.h not found`, ensure the raylib source path is included with the additional flag `-I`. 

For example, on Windows:
`gcc asteroids.c -o asteroids -lraylib -lgdi32 -lwinmm -I C:/raylib/raylib/src`