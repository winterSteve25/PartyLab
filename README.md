# Generate Project
For windows users, there are two batch files you can use depending on what compiler you are using. For linux users you can simply use a terminal.
Only do ONE of these options depending on your compiler and platform.

## Visual Studio (MSVC) Users
Visual Studio users should run

    premake-VisualStudio.bat
	
This will generate a Visual Studio project.

## Visual Studio Code (VSC) Users
Visual Studio code uses the makefile systems for MinGW-w64, gcc, or clang on Windows, linux, and OSX respectivly. Simply follow the steps below for your OS and then continue to the VSC section below. Please make sure that you have the C/C++ extensions installed for VSC. Please make sure your compiler is in your path in order to use Visual Studio Code.
	
## MinGW-w64 Users
Please make sure you have a recent version of MinGW-W64. The older versons from mingw.org will not work.
We recommend the W64Devkit. It has everything needed to build raylib. It can be downloaded from here https://github.com/skeeto/w64devkit/releases

Once you have MinGW-W64
Run the batch file.

    premake-mingw.bat

This will generate a makefile for you

## Linux users
cd to the root folder and run

    ./premake5 gmake2

This will generate a makefile for you.

## macOS users
cd to the root folder and run

    ./premake5.osx gmake2
	
This will generate a makefile for you.

# Build the game
Only do ONE of these options depending on your compiler and platform.

## Visual Studio (MSVC) Users
Double click the .sln file that was generated in the folder. From here you can use the project as normal.

## Visual Studio Code (VSC) Users
Game-premake comes with a visual studio code project file, simply open the folder in vscode and it should be setup to run the makefiles build for gcc or MinGW-w64. Just use the build, debug, or run functions in VSC

## MinGW-w64 Users
Open your compiler terminal (w64devkit if you are using it), change to the game folder and type 

    ./run
	
This will build your game
	
## Linux/macOS users
Open your terminal, change to the root folder and type.

    ./run
	
This will build your starting game template
	
	
# Building for other OpenGL targets
If you need to build for a different OpenGL version than the default (OpenGL 3.3) you can specify an openGL version in your premake command line. Just modify the bat file or add the following to your command line

## For OpenGL 1.1
--graphics=opengl11

## For OpenGL 2.1
--graphics=opengl21

## For OpenGL 4.3
--graphics=opengl43

## For OpenGLES 2.0
--graphics=opengles2

## For OpenGLES 3.0
--graphics=opengles3

---

This README file is edited from the Raylib Premake template 

# License
Copyright (c) 2020-2024 Jeffery Myers

This software is provided "as-is", without any express or implied warranty. In no event 
will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial 
applications, and to alter it and redistribute it freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not claim that you 
  wrote the original software. If you use this software in a product, an acknowledgment 
  in the product documentation would be appreciated but is not required.

  2. Altered source versions must be plainly marked as such, and must not be misrepresented
  as being the original software.

  3. This notice may not be removed or altered from any source distribution.
