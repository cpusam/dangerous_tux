cmake_minimum_required(VERSION 2.6)

project(DangerousTux)

add_definitions(-DPREFIX="${CMAKE_INSTALL_PREFIX}")
add_definitions(-I../Chora_Engine)
add_definitions(-L../Chora_Engine)

option(USE_SDL2 "Usar SDL2 ao invés de SDL1.2" ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Escolha o tipo de build" FORCE)
endif()

IF (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	add_executable(DangerousTux.exe src/dangerous_tux.cpp)
	IF (USE_SDL2)
		add_definitions(-DUSE_SDL2)
		target_link_libraries(DangerousTux.exe SDL2main SDL2 SDL2_ttf SDL2_image SDL2_mixer)
	ELSE (USE_SDL2)
		target_link_libraries(DangerousTux.exe SDLmain SDL SDL_ttf SDL_image SDL_mixer)
	ENDIF (USE_SDL2)
	
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
	add_executable(DangerousTux src/dangerous_tux.cpp)
	IF (USE_SDL2)
		add_definitions(-DUSE_SDL2)
		target_link_libraries(DangerousTux SDL2 SDL2_ttf SDL2_image SDL2_mixer)
	ELSE (USE_SDL2)
		target_link_libraries(DangerousTux SDL SDL_ttf SDL_image SDL_mixer)
	ENDIF (USE_SDL2)

	install(TARGETS DangerousTux DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
	install(DIRECTORY levels DESTINATION ${CMAKE_INSTALL_PREFIX}/share/games/dangeroustux)
	install(DIRECTORY images DESTINATION ${CMAKE_INSTALL_PREFIX}/share/games/dangeroustux)
	install(DIRECTORY fonts DESTINATION ${CMAKE_INSTALL_PREFIX}/share/games/dangeroustux)
	install(DIRECTORY sounds DESTINATION ${CMAKE_INSTALL_PREFIX}/share/games/dangeroustux)
ENDIF (${CMAKE_SYSTEM_NAME} MATCHES "Windows")



