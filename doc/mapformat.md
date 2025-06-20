# Hall of Tortured Souls Map Format

Hall of Tortured Souls loads its map data from a list of strings hardcoded in the exectuable. The full map data can be found in [data_map.c](/src/data_map.c).

Each string begins with a letter that describes its function, followed by a list of numbers. The numbers can be separated by either a space or a comma, and may have up to one decimal point.

Note that all numbers are multiplied by a scale of ten when loaded by the game.

## s
***Example***: `s 20,2 0 1`

Sets the starting point for the player. The first three parameters set the X, Y, and Z coordinates, and the last parameter sets the starting sector index.

## e
***Example***: `e -50 100 14 1`

Adds a new sector to the map. The first two numbers set the floor and ceiling heights respectively, and the last two set the floor and ceiling colours.

## w
***Example***: `w 16,80 0,80 4`

Adds a solid one-sided wall to the map. The parameters define the start X and Y coordinates, the end X and Y coordinates, and the texture number to use (0-14).

Note that solid walls don't reference sectors at all, which may make it difficult to make a converter to Doom maps.

## z, x
***Example***:
```
z 0,0 16,0
x 2 1 0,0
```

Note: `z` lines must always have an `x` line immediately afterwards.

Adds a "wall" connecting two sectors, which I've called a "thru wall" in the code.

The `z` line defines the start and end X, Y positions, and the `x` line defines the first and second sector references, the texture number to use for the upper part of the wall, and the texture number to use for the lower part of the wall.

## b
***Example***: `b 12,8`

Sets an X and Y offset to be added to all following walls until the next `b` line.
