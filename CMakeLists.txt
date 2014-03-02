cmake_minimum_required(VERSION 2.6)

project(DangerousTux)

add_definitions(-DPREFIX="${CMAKE_INSTALL_PREFIX}")

if(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Release CACHE STRING "Escolha o tipo de build" FORCE)
endif()

IF (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	add_executable(DangerousTux.exe src/dangerous_tux.cpp)
	target_link_libraries(DangerousTux.exe SDLmain SDL SDL_ttf SDL_image SDL_mixer)
	
	IF (MSVC)
    set_source_files_properties(src/dangerous_tux.cpp PROPERTIES LANGUAGE CXX)
    set_target_properties(DangerousTux.exe PROPERTIES LINKER_LANGUAGE CXX)
  ENDIF (MSVC)
  
	install(TARGETS DangerousTux.exe DESTINATION ${CMAKE_INSTALL_PREFIX}/dangeroustux)
	install(DIRECTORY levels DESTINATION ${CMAKE_INSTALL_PREFIX}/dangeroustux)
	install(DIRECTORY images DESTINATION ${CMAKE_INSTALL_PREFIX}/dangeroustux)
	install(DIRECTORY fonts DESTINATION ${CMAKE_INSTALL_PREFIX}/dangeroustux)
	install(DIRECTORY sounds DESTINATION ${CMAKE_INSTALL_PREFIX}/dangeroustux)

ELSE (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	FIND_LIBRARY(SDL_LIBRARY SDL /usr/lib)
	FIND_LIBRARY(SDL_IMAGE_LIBRARY SDL_image /usr/lib)
	FIND_LIBRARY(SDL_MIXER_LIBRARY SDL_mixer /usr/lib)
	FIND_LIBRARY(SDL_TTF_LIBRARY SDL_ttf /usr/lib)

	add_executable(DangerousTux src/dangerous_tux.cpp)
	target_link_libraries(DangerousTux m SDL SDL_ttf SDL_image SDL_mixer)

	install(TARGETS DangerousTux DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
	install(DIRECTORY levels DESTINATION ${CMAKE_INSTALL_PREFIX}/share/games/dangeroustux)
	install(DIRECTORY images DESTINATION ${CMAKE_INSTALL_PREFIX}/share/games/dangeroustux)
	install(DIRECTORY fonts DESTINATION ${CMAKE_INSTALL_PREFIX}/share/games/dangeroustux)
	install(DIRECTORY sounds DESTINATION ${CMAKE_INSTALL_PREFIX}/share/games/dangeroustux)
ENDIF (${CMAKE_SYSTEM_NAME} MATCHES "Windows")



