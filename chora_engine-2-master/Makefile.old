
CXX = g++
AR = ar
RANLIB = ranlib
CXXFLAGS = -std=c++11 -Wall -g
BASE = obj/statemachine.o obj/vect.o obj/collision.o obj/movable.o obj/util.o obj/camera.o obj/animation.o obj/sprite.o obj/background.o obj/tilemap.o obj/tilemapview.o obj/soundplayer.o obj/textureid.o obj/texturer.o  obj/sprite.o obj/Exception.o
GUI = obj/widget.o obj/writer.o obj/label.o obj/button.o obj/bar.o obj/textinput.o
GFX = obj/SDL_framerate.o
GAME = obj/Entity.o obj/Elements.o obj/Key.o
DEPS = $(BASE) $(GUI) $(GFX) $(GAME)

all: objDir libchora.a

objDir:
	mkdir -p obj
	
libchora.a: $(DEPS) Chora.hpp sdl.hpp
	$(AR) rc $@ $(DEPS)
	$(RANLIB) $@

obj/statemachine.o: statemachine.hpp statemachine.cpp
	$(CXX) -c statemachine.cpp -o $@ $(CXXFLAGS)

obj/textureid.o: textureid.hpp textureid.cpp
	$(CXX) -c textureid.cpp -o $@ $(CXXFLAGS)

obj/texturer.o: texturer.hpp texturer.cpp
	$(CXX) -c texturer.cpp -o $@ $(CXXFLAGS)

obj/vect.o: vect.hpp vect.cpp
	$(CXX) -c vect.cpp -o $@ $(CXXFLAGS)

obj/collision.o: collision.hpp collision.cpp
	$(CXX) -c collision.cpp -o $@ $(CXXFLAGS)

obj/movable.o: movable.hpp movable.cpp
	$(CXX) -c movable.cpp -o $@ $(CXXFLAGS)

obj/util.o: util.hpp util.cpp
	$(CXX) -c util.cpp -o $@ $(CXXFLAGS)

obj/camera.o: camera.hpp camera.cpp
	$(CXX) -c camera.cpp -o $@ $(CXXFLAGS)

obj/animation.o: animation.hpp animation.cpp
	$(CXX) -c animation.cpp -o $@ $(CXXFLAGS)

obj/sprite.o: sprite.hpp sprite.cpp
	$(CXX) -c sprite.cpp -o $@ $(CXXFLAGS)

obj/background.o: background.hpp background.cpp
	$(CXX) -c background.cpp -o $@ $(CXXFLAGS)

obj/tilemap.o: platform/tilemap.hpp platform/tilemap.cpp
	$(CXX) -c platform/tilemap.cpp -o $@ $(CXXFLAGS)
	
obj/tilemapview.o: platform/tilemapview.hpp platform/tilemapview.cpp
	$(CXX) -c platform/tilemapview.cpp -o $@ $(CXXFLAGS)

obj/soundplayer.o: sound/soundplayer.hpp sound/soundplayer.cpp
	$(CXX) -c sound/soundplayer.cpp -o $@ $(CXXFLAGS)

obj/Exception.o: Exception.hpp Exception.cpp
	$(CXX) -c Exception.cpp -o $@ $(CXXFLAGS)

obj/SDL_framerate.o: SDL_gfx/SDL_framerate.hpp SDL_gfx/SDL_framerate.cpp
	$(CXX) -c SDL_gfx/SDL_framerate.cpp -o $@ $(CXXFLAGS)

#################################################
obj/widget.o: gui/widget.hpp gui/widget.cpp
	$(CXX) -c gui/widget.cpp -o $@ $(CXXFLAGS)

obj/writer.o: gui/writer.hpp gui/writer.cpp
	$(CXX) -c gui/writer.cpp -o $@ $(CXXFLAGS)

obj/label.o: gui/label.hpp gui/label.cpp
	$(CXX) -c gui/label.cpp -o $@ $(CXXFLAGS)

obj/button.o: gui/button.hpp gui/button.cpp
	$(CXX) -c gui/button.cpp -o $@ $(CXXFLAGS)

obj/bar.o: gui/bar.hpp gui/bar.cpp
	$(CXX) -c gui/bar.cpp -o $@ $(CXXFLAGS)

obj/textinput.o: gui/textinput.hpp gui/textinput.cpp
	$(CXX) -c gui/textinput.cpp -o $@ $(CXXFLAGS)

#################################################
obj/Key.o: game/Key.hpp game/Key.cpp
	$(CXX) -c game/Key.cpp -o $@ $(CXXFLAGS)

obj/Entity.o: game/Entity.hpp game/Entity.cpp
	$(CXX) -c game/Entity.cpp -o $@ $(CXXFLAGS)

obj/Elements.o: game/Elements.hpp game/Elements.cpp
	$(CXX) -c game/Elements.cpp -o $@ $(CXXFLAGS)



clean: clean_objs
	rm -f libchora.a
	
clean_objs:
	rm -f obj/*.o
	rm -f *~ platform/*~ gui/*~ game/*~ SDL_gfx/*~ sound/*~
