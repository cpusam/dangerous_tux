
file(GLOB chora_sources "*.cpp" "gui/*.cpp" "platform/*.cpp" "sound/*.cpp" "SDL_gfx/*.cpp")
file(GLOB chora_headers "*.hpp" "gui/*.hpp" "platform/*.hpp" "sound/*.hpp" "SDL_gfx/*.hpp")

#project(chora)

include_directories("gui" "platform" "sound" "SDL_gfx")

add_library(DangerousTux STATIC ${chora_sources})

if(MSVC)
  set_source_files_properties(${chora_sources} PROPERTIES LANGUAGE CXX)
  set_target_properties(DangerousTux PROPERTIES LINKER_LANGUAGE CXX)
endif(MSVC)

set_target_properties(DangerousTux PROPERTIES VERSION 6.2.0)


