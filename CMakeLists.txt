cmake_minimum_required(VERSION 2.6)

project(DangerousTux)

add_definitions(-DPREFIX="${CMAKE_INSTALL_PREFIX}")

IF (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	add_executable(DangerousTux.exe dangerous_tux.cpp)
	target_link_libraries(DangerousTux.exe SDLmain SDL SDL_ttf SDL_image)
	
	IF (MSVC)
    set_source_files_properties(dangerous_tux.cpp PROPERTIES LANGUAGE CXX)
    set_target_properties(DangerousTux.exe PROPERTIES LINKER_LANGUAGE CXX)
  ENDIF (MSVC)
  
	install(TARGETS DangerousTux.exe DESTINATION ${CMAKE_INSTALL_PREFIX}/dangeroustux)
	install(DIRECTORY levels DESTINATION ${CMAKE_INSTALL_PREFIX}/dangeroustux)
	install(DIRECTORY images DESTINATION ${CMAKE_INSTALL_PREFIX}/dangeroustux)
	install(DIRECTORY fonts DESTINATION ${CMAKE_INSTALL_PREFIX}/dangeroustux)
ELSE (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	add_executable(DangerousTux dangerous_tux.cpp)
	target_link_libraries(DangerousTux m SDL SDL_ttf SDL_image)

	install(TARGETS DangerousTux DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
	install(DIRECTORY levels DESTINATION ${CMAKE_INSTALL_PREFIX}/share/games/dangeroustux)
	install(DIRECTORY images DESTINATION ${CMAKE_INSTALL_PREFIX}/share/games/dangeroustux)
	install(DIRECTORY fonts DESTINATION ${CMAKE_INSTALL_PREFIX}/share/games/dangeroustux)
ENDIF (${CMAKE_SYSTEM_NAME} MATCHES "Windows")



