# Project overview
The project is a simple combat game.
For graphics I used ncurses library and developed ASCII art characters.
The game implements simple UI.
For the purpose of loading assets I developed simple markup language.

# Markup language
Language is specific to the implementation and can be hard to reuse in further projects.
## How it works
First it looks for name tag (f.e. Map, Player).
Then if it's Map, algorithms is as follows:
* read lines of map until EOM markup
* save map
For Player: (read animations)
* read number (it's number of animation frames)
* read lines of animation's frame until EFM (end of frame) markup
* jump to first instruction and repeat until EOC markup(end of clip)
* jump to first instruction and repeat until EOA (end of animation)
* read animations until END markup

Parsing finishes when EOF is found

# CMake
## How to create Debug and Release mode in cmake

### create directiories "Debug" and "Release"
in each of directory run:
#### in Debug/
    cmake -DCMAKE_BUILD_TYPE=Debug ..
#### in Release/
    cmake -DCMAKE_BUILD_TYPE=Release ..
#### or run interactive cmake
    ccmake ..
