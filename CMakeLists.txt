cmake_minimum_required(VERSION 2.6)

project(DangerousTux)

add_definitions(-I../chora_engine)
add_definitions(-L../chora_engine)
LINK_DIRECTORIES(../chora_engine/)
LINK_DIRECTORIES(../chora_engine/)
include_directories(src)
include_directories(../chora_engine)

file(GLOB_RECURSE SRCS src/*.cpp)
file(GLOB_RECURSE HDRS *.h)


IF (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	add_executable(DangerousTux.exe ${SRCS})
	target_link_libraries(DangerousTux.exe SDL2main SDL2 SDL2_ttf SDL2_image SDL2_mixer)

	IF (MSVC)
    	set_source_files_properties(${SRCS} PROPERTIES LANGUAGE CXX)
    	set_target_properties(DangerousTux.exe PROPERTIES LINKER_LANGUAGE CXX)
  	ENDIF ()
  
	install(TARGETS DangerousTux.exe DESTINATION ${CMAKE_INSTALL_PREFIX}/dangeroustux)
	install(DIRECTORY levels DESTINATION ${CMAKE_INSTALL_PREFIX}/dangeroustux)
	install(DIRECTORY images DESTINATION ${CMAKE_INSTALL_PREFIX}/dangeroustux)
	install(DIRECTORY fonts DESTINATION ${CMAKE_INSTALL_PREFIX}/dangeroustux)
	install(DIRECTORY sounds DESTINATION ${CMAKE_INSTALL_PREFIX}/dangeroustux)

ELSE ()
	#adicionado para compilar com gcc
	add_definitions("--std=c++11")
	add_executable(DangerousTux ${SRCS})
	target_link_libraries(DangerousTux SDL2 SDL2_ttf SDL2_image SDL2_mixer chora)

	install(TARGETS DangerousTux DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
	install(DIRECTORY levels DESTINATION ${CMAKE_INSTALL_PREFIX}/share/games/dangeroustux)
	install(DIRECTORY images DESTINATION ${CMAKE_INSTALL_PREFIX}/share/games/dangeroustux)
	install(DIRECTORY fonts DESTINATION ${CMAKE_INSTALL_PREFIX}/share/games/dangeroustux)
	install(DIRECTORY sounds DESTINATION ${CMAKE_INSTALL_PREFIX}/share/games/dangeroustux)
ENDIF (${CMAKE_SYSTEM_NAME} MATCHES "Windows")



