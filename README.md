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

Pass `--fullscreen` to run fullscreen instead of in a 1024x768 window.

## Controls

| Input               | Action                                        |
| ------------------- | --------------------------------------------- |
| Mouse               | Turn the ship                                 |
| Up / Down arrows    | Thrust forward / backward                     |
| Left / Right arrows | Strafe                                        |
| Left mouse button   | Fire a laser                                  |
| Right mouse button  | Fire a homing laser at the asteroid ahead     |
| Alt+Q               | Quit                                          |

## Credits

The HUD font is Liberation Mono Bold, licensed under the SIL Open Font
License (see `LiberationMono-LICENSE.txt`).
