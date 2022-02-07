# Conway's Game of life in 3D

Conways game of lif in 3D using C++ and OpenGL.

The rules of the game are life 4 5 5 5, that is
the rule set in which an alive cell will stay alive in the
next generation if it has n live neighbors and 4 <= n <= 5 and a dead cell
will become alive in the next generation if 5 <= n <= 5.

The choice of this rule as well as the more complicated structures come from [Carter Bays, *Candidates for the Game of Life in Three Dimensions*](https://content.wolfram.com/uploads/sites/13/2018/02/01-3-1.pdf)

## 📷 Screenshot
🖼


## ⚙ Controls

> The controls are *heavely* inspired by minecraft's commands.

To move around use the `W`, `A`, `S` and `D` keys.

Use `Space` and `Shift` to go up and down.

Place voxels with `R click`, Remove voxels with `L click`.

Access the debug menu through `F3`.

Exit with `Esc`.

Type commands with `/`.

Implemented commands are

- `start` which starts the simulation

- `stop` which stops the simulation

- `reset` which stops the simulation and resets all the voxels to their state before the simulation started

 
## ⚡ Installation

**I would strongly recommend building the binaries from the source code using CMake.**
However I also have included a zip containing the output of the build process (for windows x64).

