# OpenSiv3D for Web

## Requirements

To build OpenSiv3D for Web, CMake and emscripten installation are required.

The dependent libraries are as follows:

- OpenGLES 3.0 (`-s FULL_ES3`)
- libpng (`-s USE_LIBPNG=1`)
- FreeType2, HarfBuzz (`-s USE_FREETYPE=1, -s USE_HARFBUZZ=1`)
- OpenAL
- libogg, libvorbis (`-s USE_OGG=1, -s USE_VORBIS=1`)
- Boost (`-s USE_BOOST_HEADERS=1`)
- zlib (`-s USE_ZLIB=1`)

Not currently required dependencies are:

- OpenCV4

## Compiling OpenSiv3D

1. Clone source and change branch.
    1. `git clone https://github.com/Siv3D/OpenSiv3D.git`
    1. `cd OpenSiv3D`
1. Make a directory to Build. (In this example, make `Web/Build`).
    1. `mkdir Web/Build`
    1. `cd Web/Build`
1. Compile OpenSiv3D. If build succeeds, `libSiv3D.a` is obtained.
    1. `emcmake cmake -DCMAKE_BUILD_TYPE=Release ..`
    1. `emmake make`

### Note

- Using `-DCMAKE_BUILD_TYPE=Debug`, it is compiled with `-g3 -O0`.
- The above example uses `make`, but you can also compile with `ninja`.
- Set `CC` and `CXX` before running `cmake` to specify the compiler.

## Compiling an application

Sample files required to compile an application are in `Web/App` directory.
You can build the sample with `cmake` like OpenSiv3D.

- `Main.cpp`
  - Sample source of the application.
- `CMakeLists.txt`
  - Sample of `CMakeLists.txt` to build the application.
- `resources/` directory
  - It contains resource files required by the application.

### Note

- Change the path of `libSiv3D.a` and include directories in `CMakeLists.txt` properly, when you copy or move `App` directory.
- Make sure that the `resources/` directory is in the same directory as the executable file when runnning the application.

## Supported Features
