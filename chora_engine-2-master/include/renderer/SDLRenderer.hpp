//
// Created by thiago on 12/10/18.
//

#ifndef CHORA_RENDER_HPP
#define CHORA_RENDER_HPP

#include "renderer.hpp"

class SDLRenderer : public Renderer {
    SDL_Renderer *renderer;
    SDL_Window * window;
public:

    SDLRenderer(SDL_Window *window, unsigned int flags);

    SDL_Window *getWindow() const;

    SDL_Renderer *getRenderer() const;

    void clear(uint8_t r, uint8_t g, uint8_t b, uint8_t a) override;

    void drawRect(SDL_Rect &rect, uint8_t r, uint8_t g, uint8_t b, uint8_t a) override;

    void drawTexture(void *texture, const SDL_Rect &src, const SDL_Rect &dest) override;

    unsigned int getFlags() const override;

};


#endif //CHORA_RENDER_HPP
