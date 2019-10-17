Gloom
=====

*Requires the small3d framework*

You are in a gloomy place and some anthropoids are trying to kill you.
Kill them first.

Move with the arrows. Shoot with the spacebar. Escape to quit the game.

Building
--------
Create a directory called *deps* inside the game project's root directory.
Build the small3d framework and copy the resulting *include*, *lib* and *shaders* directories from the small3d *build* directory to the *deps* directory created. Then from the game project's root directory, do the following:
	
	mkdir build
    cd build
    cmake ..
    cmake --build .
	
On Windows, you need to execute `cmake .. -G"MinGW Makefiles"`, or with the preferred Visual Studio configuration (e.g. `cmake .. -G"Visual Studio 16 2019" -A x64`). Also if using Visual Studio, make sure to run build under the configuration that small3d was built with, e.g. `cmake --build . --config Debug`.

In order to run the game, execute:
	
	cd bin
	./groom (or groom.exe if you are on Windows)
	
For building the game with the OpenGL edition of small3d, the procedure is the same. However you need to perform the following changes beforehand:

In CMakeLists.txt:

	- Delete the find_package commands that look for Vulkan:
		find_package(Vulkan REQUIRED)
	- Add a find_package command for OpenGL and one for GLEW:
		find_package(OpenGL REQUIRED)
		find_package(GLEW REQUIRED)
	
In src/CMakeLists.txt:

	- In the target_include_directories command, replace ${Vulkan_INCLUDE_DIR} with ${OPENGL_INCLUDE_DIR}
	- In the target_link_libraries command, remove ${Vulkan_LIBRARIES} and add ${GLEW_LIBRARIES} and
	  ${OPENGL_LIBRARIES} in that order, right underneath ${SMALL3D_LIBRARY}.
