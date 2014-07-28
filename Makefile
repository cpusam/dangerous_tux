
CXX = g++
<<<<<<< HEAD
CXXFLAGS = -g -w -march=i386 -Wall#-fno-stack-protector
LDLIBS = src/chora_engine/chora.a `sdl-config --cflags  --libs` -lm -lSDL_image -lSDL_ttf -lSDL_mixer
LDLIBS_SDL2 = src/chora_engine/chora-SDL2.a `sdl2-config --cflags --libs` -lm -lSDL2_image -lSDL2_ttf -lSDL2_mixer
DEPS = src/video.o src/save.o src/gameevent_manager.o src/gameentity.o src/score.o src/highscore.o src/gamesignal.o src/jetpack.o src/gun.o src/player.o src/flyeralien.o src/circlealien.o src/walkeralien.o src/level.o src/gamemenu.o src/gamecredits.o src/gameintroduction.o src/gametransition.o src/gameover.o src/gametitle.o src/gamescreen.o
=======
CXXFLAGS = -g -w -march=i386 #-fno-stack-protector
LDLIBS = `sdl-config --cflags  --libs` -lm -lSDL_image -lSDL_ttf -lSDL_mixer
LDLIBS_SDL2 = `sdl2-config --cflags --libs` -lm -lSDL2_image -lSDL2_ttf -lSDL2_mixer
DEPS = src/dangerous_tux.cpp src/gameentity.hpp src/player.hpp src/aliensdef.hpp src/walkeralien.hpp src/flyeralien.hpp src/circlealien.hpp src/jetpack.hpp src/gun.hpp src/level.hpp src/gamescreen.hpp src/gameover.hpp src/gamecredits.hpp src/gametransition.hpp src/gameintroduction.hpp src/gametitle.hpp src/gameevent_manager.hpp src/highscore.hpp src/score.hpp src/savegame.hpp src/gamesignal.hpp src/gamevideo.hpp src/gamemenu.hpp
>>>>>>> 1fba5f672f27675ef61fc15b644b461379515813

all: DangerousTux

DangerousTux: $(DEPS)
<<<<<<< HEAD
	$(CXX) -o $@ src/dangerous_tux.cpp $(DEPS) $(CXXFLAGS) $(LDLIBS)

DangerousTux.SDL2: $(DEPS)
	$(CXX) -o DangerousTux src/dangerous_tux.cpp -DUSE_SDL2 $(DEPS) $(CXXFLAGS) $(LDLIBS_SDL2)

# $(CXX) -c src/.cpp -o $@ $(CXXFLAGS)
src/video.o: src/video.hpp src/video.cpp
	$(CXX) -c src/video.cpp -o $@ $(CXXFLAGS)

src/save.o: src/save.hpp src/save.cpp
	$(CXX) -c src/save.cpp -o $@ $(CXXFLAGS)

src/gameevent_manager.o: src/gameevent_manager.hpp src/gameevent_manager.cpp
	$(CXX) -c src/gameevent_manager.cpp -o $@ $(CXXFLAGS)

src/gameentity.o: src/gameentity.hpp src/gameentity.cpp
	$(CXX) -c src/gameentity.cpp -o $@ $(CXXFLAGS)

src/score.o: src/score.hpp src/score.cpp
	$(CXX) -c src/score.cpp -o $@ $(CXXFLAGS)
	
src/highscore.o: src/highscore.hpp src/highscore.cpp
	$(CXX) -c src/highscore.cpp -o $@ $(CXXFLAGS)

src/gamesignal.o: src/gamesignal.hpp src/gamesignal.cpp
	$(CXX) -c src/gamesignal.cpp -o $@ $(CXXFLAGS)

src/jetpack.o: src/jetpack.hpp src/jetpack.cpp
	$(CXX) -c src/jetpack.cpp -o $@ $(CXXFLAGS)

src/gun.o: src/gun.hpp src/gun.cpp
	$(CXX) -c src/gun.cpp -o $@ $(CXXFLAGS)

src/player.o: src/player.hpp src/player.cpp
	$(CXX) -c src/player.cpp -o $@ $(CXXFLAGS)

src/flyeralien.o: src/flyeralien.hpp src/flyeralien.cpp
	$(CXX) -c src/flyeralien.cpp -o $@ $(CXXFLAGS)

src/circlealien.o: src/circlealien.hpp src/circlealien.cpp
	$(CXX) -c src/circlealien.cpp -o $@ $(CXXFLAGS)

src/walkeralien.o: src/walkeralien.hpp src/walkeralien.cpp
	$(CXX) -c src/walkeralien.cpp -o $@ $(CXXFLAGS)

src/level.o: src/level.hpp src/level.cpp
	$(CXX) -c src/level.cpp -o $@ $(CXXFLAGS)

src/gamemenu.o: src/gamemenu.hpp src/gamemenu.cpp
	$(CXX) -c src/gamemenu.cpp -o $@ $(CXXFLAGS)
	
src/gamecredits.o: src/gamecredits.hpp src/gamecredits.cpp
	$(CXX) -c src/gamecredits.cpp -o $@ $(CXXFLAGS)

src/gameintroduction.o: src/gameintroduction.hpp src/gameintroduction.cpp
	$(CXX) -c src/gameintroduction.cpp -o $@ $(CXXFLAGS)

src/gametransition.o: src/gametransition.hpp src/gametransition.cpp
	$(CXX) -c src/gametransition.cpp -o $@ $(CXXFLAGS)

src/gameover.o: src/gameover.hpp src/gameover.cpp
	$(CXX) -c src/gameover.cpp -o $@ $(CXXFLAGS)

src/gametitle.o: src/gametitle.hpp src/gametitle.cpp
	$(CXX) -c src/gametitle.cpp -o $@ $(CXXFLAGS)

src/gamescreen.o: src/gamescreen.hpp src/gamescreen.cpp
	$(CXX) -c src/gamescreen.cpp -o $@ $(CXXFLAGS)

clean:
	@rm -f DangerousTux src/*.o

=======
	$(CXX) -o $@ src/dangerous_tux.cpp $(CXXFLAGS) $(LDLIBS)

DangerousTux.SDL2: $(DEPS)
	$(CXX) -o DangerousTux src/dangerous_tux.cpp -DUSE_SDL2 $(CXXFLAGS) $(LDLIBS_SDL2)

clean:
	-rm -f DangerousTux
>>>>>>> 1fba5f672f27675ef61fc15b644b461379515813
