# SGAL V0.1
The **S**imple **G**raphics and **A**udio **L**ibrary is a straight-forward object-oriented library built in C++ meant to give the user a streamlined way of creating robust 3D graphics through a simple and small API. It uses OpenGL for the low-level rendering operations, and all window handling is done in the library. It is cross platform.

All dependencies are automatically downloaded and synced when you compile using CMake.

### Links
Github: [Link](https://github.com/maxortner01/SGAL/)

##### Relevant Links
GLEW: [Link](https://github.com/nigels-com/glew)

CMake: [Link](https://cmake.org/download/)

## Compilation

SGAL uses `cmake` to compile. It needs a `build` folder and `lib` folder. Change your directory to the build folder and run `cmake ..` (with your additional parameters if needed) and it should get to work compiling the library. All files needed for linking will be found in the `lib` folder, and if you don't want to compile the client application simply disable the `COMPILE_CLIENT` option in CMake.

### Dependencies
SGAL uses only the `GLEW` library as a dependancy. For now it pulls from the `cmake-glew` repository since it includes the `glew.c` file which is needed for easy compilation until I can figure out the more official repo.

## Changelog

### Version 0.1 - 4/29/2020

- Complete rewrite.
- Foundation for UI rendering.
- Now does in-house window handling. Right now it only supports windows, I will do an X11 implementation soon.
- Basic serialization so you can generate RawModels and write/read them from files.
- In-house shaders for 2D and 3D rendering.
- Supports multiple framebuffers.
- Each render takes a different context, meaning you can change cameras and render with different lights.
- Architecture looks prettier.

#### Planned Features

- SingleModel class that represents a RawModel and Model in one fell swoop.
- Actual documentation.
- Vulkan instead of OpenGL.
- X11 implementation.
- Normal maps and more PBR-like lighting calculations.

## Author

Max Ortner