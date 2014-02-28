
CXX = g++
CXXFLAGS = -g -march=i386 #-fno-stack-protector
<<<<<<< HEAD
LDLIBS = -lm -lSDL -lSDL_image -lSDL_ttf -lSDL_mixer
DEPS = src/dangerous_tux.cpp src/gameentity.hpp src/player.hpp src/aliens.hpp src/jetpack.hpp src/gun.hpp src/level.hpp src/gamescreen.hpp src/gameover.hpp src/gamecredits.hpp src/gametransition.hpp src/gameintroduction.hpp src/gametitle.hpp src/gameevent_manager.hpp src/highscore.hpp src/score.hpp src/savegame.hpp src/gamesignal.hpp src/gamevideo.hpp
=======
LDLIBS = -lm `sdl-config --cflags --static-libs` -lSDL -lSDL_image -lSDL_ttf -lSDL_mixer
DEPS = src/dangerous_tux.cpp src/gameentity.hpp src/player.hpp src/aliens.hpp src/jetpack.hpp src/gun.hpp src/level.hpp src/gamescreen.hpp src/gameentity.hpp src/gameevent_manager.hpp src/highscore.hpp src/score.hpp src/savegame.hpp src/gamesignal.hpp src/gamevideo.hpp
>>>>>>> 9aeed8c5044214ec82e64e611b1c7acd44cc4897

all: DangerousTux

DangerousTux: $(DEPS)
	$(CXX) -o $@ src/dangerous_tux.cpp $(CXXFLAGS) $(LDLIBS)

clean:
	rm -f DangerousTux
