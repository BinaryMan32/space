# Space

A top-down space game similar to asteroids.
Originally implemented using Visual C++, OpenGL, DirectInput, and DirectSound.
Now uses SDL3 for windowing, input, and sound, and SDL3_ttf for text.

## Install Prerequisites

```sh
sudo apt install cmake libsdl3-dev libsdl3-ttf-dev libgl-dev libglu1-mesa-dev
```

## Building

```sh
cmake -B build
cmake --build build -j8
```

## Running

The images and sounds are copied next to the executable, so run it from there:

```sh
cd build/bin
./SpaceGame
```
