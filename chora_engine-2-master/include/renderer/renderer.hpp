#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "sdl.hpp"

class Renderer
{
	public:
		enum Flags: unsigned int
		{
			RENDER_DEFAULT=1,
			RENDER_ACCELERATED=2,
		};
	public:
		Renderer (  );

		int getId (  ) const;
		virtual unsigned int getFlags (  ) const;

		virtual void clear(uint8_t r, uint8_t g, uint8_t b, uint8_t a) = 0;
		virtual void drawRect(SDL_Rect &rect, uint8_t r, uint8_t g, uint8_t b, uint8_t a) = 0;
		virtual void drawTexture ( void * texture, const SDL_Rect & src, const SDL_Rect & dest ) = 0;

	protected:
		//flags do renderer
		unsigned int flags;
	
	private:
		//id único do renderer
		const int id;
		//contador de ids
		static int countID;
};

#endif