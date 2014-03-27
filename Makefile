
CXX = g++
CXXFLAGS = -g -w -march=i386 #-fno-stack-protector
LDLIBS = `sdl-config --cflags --static-libs` -lm -lSDL_image -lSDL_ttf -lSDL_mixer
LDLIBS_SDL2 = `sdl2-config --cflags --static-libs` -lm -lSDL2_image -lSDL2_ttf -lSDL2_mixer
DEPS = src/dangerous_tux.cpp src/gameentity.hpp src/player.hpp src/aliensdef.hpp src/walkeralien.hpp src/flyeralien.hpp src/circlealien.hpp src/jetpack.hpp src/gun.hpp src/level.hpp src/gamescreen.hpp src/gameover.hpp src/gamecredits.hpp src/gametransition.hpp src/gameintroduction.hpp src/gametitle.hpp src/gameevent_manager.hpp src/highscore.hpp src/score.hpp src/savegame.hpp src/gamesignal.hpp src/gamevideo.hpp src/gamemenu.hpp

all: DangerousTux

DangerousTux: $(DEPS)
	$(CXX) -o $@ src/dangerous_tux.cpp $(CXXFLAGS) $(LDLIBS)

DangerousTux.SDL2: $(DEPS)
	$(CXX) -o DangerousTux src/dangerous_tux.cpp -DUSE_SDL2 $(CXXFLAGS) $(LDLIBS_SDL2)

clean:
	rm -f DangerousTux
