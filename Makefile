
CXX = g++
CXXFLAGS = -std=c++11 -DUSE_SDL2 -g -w -Wall -Og -I./../Chora_Engine
LDLIBS = ../Chora_Engine/libchora.a `sdl2-config --cflags  --libs` -lm -lSDL2_image -lSDL2_ttf -lSDL2_mixer
DEPS = src/save.o src/gameevent_manager.o src/gameentity.o src/score.o src/highscore.o src/gamesignal.o src/jetpack.o src/gun.o src/player.o src/flyeralien.o src/gyroalien.o src/walkeralien.o src/level.o src/gamemenu.o src/gamecredits.o src/gameintroduction.o src/gametransition.o src/gameover.o src/gametitle.o src/gamescreen.o #src/video.o

all: DangerousTux 

DangerousTux: $(DEPS) ../Chora_Engine/libchora.a
	$(CXX) -o $@ src/dangerous_tux.cpp -DUSE_SDL2 $(DEPS) $(CXXFLAGS) $(LDLIBS)

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

src/gyroalien.o: src/gyroalien.hpp src/gyroalien.cpp
	$(CXX) -c src/gyroalien.cpp -o $@ $(CXXFLAGS)

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

clean_objs:
	@rm -f src/*.o
