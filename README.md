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

#
compile with: `-lraylib -lgdi32 -lwinmm`

optional: `-Wall -std=c99`

If there is a compiler error, `error: incompatible types when assigning to type 'Vector2' from type 'int'`, or error related to raymath.h not being found, add the additional flag `-I` with the path to the raylib source. for example in windows: `-I c:/raylib/raylib/src`