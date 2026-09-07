This is a very simple clone of Stardew Valley's "Journey of the Prairie King"
minigame. Made with Raylib and C++.

# Controls:
WASD to move
Arrow keys to shoot

# Current features:
- Map with some random bushes
- Can move and collide with stuff
- Enemies that follow the player
- Can shoot and kill enemies
- Game resets when you die

# Build instructions:
**Linux**:
You'll need CMake and GCC. Also I think you will need the environment variable
$CXX to be set to g++ in your shell.
1. Clone the repository and `cd` into the folder
2. Run `cmake -B build` to create a build directory
3. `cd` into the new `/build` directory
4. Build the project with `cmake --build .`
5. Run `raylib_prairie_king`

**Windows**:
TODO
