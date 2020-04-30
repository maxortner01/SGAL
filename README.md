# SGAL
The **S**imple **G**raphics and **A**udio **L**ibrary is a straight-forward object-oriented library built in C++ meant to give the user a streamlined way of creating robust 3D graphics through a simple and small API. It uses OpenGL for the low-level rendering operations, and all window handling is done in the library. It is cross platform.

### Dependencies
SGAL uses only the `GLEW` library as a dependancy. For now it pulls from the `cmake-glew` repository since it includes the `glew.c` file which is needed for easy compilation until I can figure out the more official repo.

All dependencies are automatically downloaded and synced when you compile using CMake.

### Links
Github: [Link](https://github.com/maxortner01/SGAL/)

##### Relevant Links
GLEW: [Link](https://github.com/nigels-com/glew)

CMake: [Link](https://cmake.org/download/)

## Compilation

SGAL uses `cmake` to compile. It needs a `build` folder and `lib` folder. Change your directory to the build folder and run `cmake ..` (with your additional parameters if needed) and it should get to work compiling the library. All files needed for linking will be found in the `lib` folder, and if you don't want to compile the client application simply disable the `COMPILE_CLIENT` option in CMake.

## Author

Max Ortner