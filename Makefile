
CXX = g++
LDLIBS = -lm `sdl-config --cflags --static-libs` -lSDL_image -lSDL_ttf

all: DangerousTux

DangerousTux: dangerous_tux.cpp gameentity.hpp player.hpp aliens.hpp jetpack.hpp gun.hpp level.hpp gamescreen.hpp gameentity.hpp gameevent_manager.hpp highscore.hpp score.hpp savegame.hpp gamesignal.hpp gamevideo.hpp
	$(CXX) -o $@ dangerous_tux.cpp -g -march=i386 $(LDLIBS)

clean:
	rm -f DangerousTux
