
CXX = g++
CXXFLAGS = -g -march=i386 #-fno-stack-protector
LDLIBS = -lm -ldl -lSDL -lSDL_image -lSDL_ttf -lSDL_mixer
DEPS = src/dangerous_tux.cpp src/gameentity.hpp src/player.hpp src/aliens.hpp src/jetpack.hpp src/gun.hpp src/level.hpp src/gamescreen.hpp src/gameover.hpp src/gamecredits.hpp src/gametransition.hpp src/gameintroduction.hpp src/gametitle.hpp src/gameevent_manager.hpp src/highscore.hpp src/score.hpp src/savegame.hpp src/gamesignal.hpp src/gamevideo.hpp

all: DangerousTux

DangerousTux: $(DEPS)
	$(CXX) -o $@ src/dangerous_tux.cpp $(CXXFLAGS) $(LDLIBS)

clean:
	rm -f DangerousTux
