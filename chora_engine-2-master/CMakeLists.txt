cmake_minimum_required(VERSION 2.8)
PROJECT(chora CXX)

SET(COMPILE_TO_WINDOWS FALSE)
SET(RELEASE FALSE)
SET(USE_CLANG FALSE)
SET(CLANG_PREFIX "/opt/clangllvm7/")
SET(DEBUG TRUE)

IF (USE_CLANG)
	SET(CMAKE_CXX_COMPILER "${CLANG_PREFIX}/bin/clang++")
ENDIF (USE_CLANG)

#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

IF (CMAKE_VERSION VERSION_LESS "3.1")
	IF (NOT USE_CLANG AND NOT COMPILE_TO_WINDOWS)
		SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
	ELSE (NOT USE_CLANG AND NOT COMPILE_TO_WINDOWS)
		SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
		SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lm -lc -lstdc++ -lc++ -lc++abi")
	ENDIF (NOT USE_CLANG AND NOT COMPILE_TO_WINDOWS)
ELSE (CMAKE_VERSION VERSION_LESS "3.1")
	SET (CMAKE_CXX_STANDARD 11)
	SET(CMAKE_CXX_STANDARD_REQUIRED ON)
	#add_compile_options("-std=c++11")
ENDIF (CMAKE_VERSION VERSION_LESS "3.1")

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -DNO_THREAD_SAFE")
if (DEBUG)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -p")
	SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_GLIBCXX_DEBUG")
endif (DEBUG)

file(GLOB_RECURSE SRCS "src/*.cpp" "game/*.cpp" "gui/*.cpp" "platform/*.cpp" "SDL_gfx/*.cpp" "sound/*.cpp" "src/renderer/*.cpp")
file(GLOB_RECURSE HDRS "Chora.hpp" "sdl.hpp" "include/*.hpp" "game/*.hpp" "gui/*.hpp" "platform/*.hpp" "SDL_gfx/*.hpp" "sound/*.hpp" "include/renderer/*.hpp")

add_definitions("-DNO_THREAD_SAFE")

include_directories(. include game platform gui sound SDL_gfx)
add_library(chora ${HDRS} ${SRCS})


IF (WIN32 OR COMPILE_TO_WINDOWS)
	SET(CMAKE_SYSTEM_NAME Windows)
	SET(TOOLCHAIN_PREFIX /usr/bin/x86_64-w64-mingw32)
	#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

	# cross compilers to use for C, C++ and Fortran
	#SET(CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}-gcc)
	SET(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}-g++)
	#SET(CMAKE_Fortran_COMPILER ${TOOLCHAIN_PREFIX}-gfortran)
	SET(CMAKE_RC_COMPILER ${TOOLCHAIN_PREFIX}-windres)

	# target environment on the build host system
	SET(CMAKE_FIND_ROOT_PATH /usr/${TOOLCHAIN_PREFIX})

	# modify default behavior of FIND_XXX() commands
	SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
	SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
	SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

	include_directories(SDL2/include)
	add_definitions("-L./SDL2/lib/")
	target_link_libraries(chora SDL2main SDL2 SDL2_ttf SDL2_image SDL2_mixer m)
ENDIF(WIN32 OR COMPILE_TO_WINDOWS)

IF (UNIX AND NOT COMPILE_TO_WINDOWS)
	target_link_libraries(chora SDL2 SDL2_ttf SDL2_image SDL2_mixer m)
ENDIF(UNIX AND NOT COMPILE_TO_WINDOWS)


