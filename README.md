# Hall of Tortured Souls

This is the decompiled source code for the hidden "Hall of Tortured Souls" game in Excel 95.

## Standalone Build

A standalone version of the game that runs on modern Windows can be compiled from the source code.

This build makes a few changes to the original game:
- A main function is added to directly call the game setup and loop function.
- The main loop now sleeps for 20ms, otherwise the game is unplayably fast on modern CPUs.
- Clicking outside of the window no longer quits the game.

There are two options for compiling the code:
### Visual Studio
Simply open the Visual Studio solution file and build the project.

### w64devkit
Open a [w64devkit](https://github.com/skeeto/w64devkit) terminal and run `make` in the `src` directory.

## Discoveries
Here are a few things I've discovered while looking around in the code that I haven't seen mentioned anywhere else:
- There is a second cheat code **XLU2** that enables a "Random Thoughts" section at the end of the scrolling credits texture!
- Holding CTRL while moving allows you to fly across ledges, which makes crossing the secret zigzag path much easier.
- The cheat code to open the wall is often said to be "EXCELKFA", but you only need to type **XLKFA** to activate the secret.
- The game has some unfinished floor texture mapping code, which is used but immediately drawn over with solid colour.

I've also documented the text-based map format used by the game, which can be found [here](doc/mapformat.md).

## Contributing
This is my first reverse engineering project, so any help is appreciated. This includes documenting the behaviour of functions, improving code accuracy, giving functions and variables better names, and cleaning up functions for better readability.

The code is based off of Ghidra's decompiler output, and I would like to clean everything up to be more readable. The names of functions and variables also may not be accurate to what they represent.

There is also the big project of reverse engineering the floating point math code (see the comment in [floating.c](src/floating.c)), which I've replaced with simplified code for now.
