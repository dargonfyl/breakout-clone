# Breakout Clone
The Breakout clone from [LearnOpenGL](https://learnopengl.com/In-Practice/2D-Game/Breakout), with a bunch of my own additions.

## Requirements
You will need:
- OpenGL version 3.3+ capable system
- CMake installed

## Building
Run the following:
```
$ git clone --recursive https://github.com/dargonfyl/breakout-clone.git
$ cd breakout-clone
$ mkdir build
$ cd build
$ cmake .. [-G your-target-generator] [-DCMAKE_BUILD_TYPE=Release/Debug]
$ cmake --build .
```

__NOTE:__ This project has a lot of assertions that only work in Debug mode. It is recommended that you start in Debug mode for this reason.

## License
Licensed under The Unlicense, that is, the code is in the public domain (you can do whatever you want with it), and there is no warranty/liability.

## Acknowledgements
- [Joey de Vries](https://github.com/JoeyDeVries), who made Learn OpenGL
- [Tomasz Gałaj](https://github.com/Shot511), who made [this](https://shot511.github.io/2018-05-29-how-to-setup-opengl-project-with-cmake/) handy CMake tutorial.
