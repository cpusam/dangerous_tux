//
// Created by thiago on 12/10/18.
//
#include <include/renderer/SDLRenderer.hpp>

#include "renderer/SDLRenderer.hpp"

SDLRenderer::SDLRenderer(SDL_Window *window, unsigned int flags)
        : Renderer() {
    this->window = window;
    this->renderer = SDL_CreateRenderer(this->window, 0, flags);
}

SDL_Window *SDLRenderer::getWindow() const {
    return window;
}

SDL_Renderer *SDLRenderer::getRenderer() const {
    return renderer;
}

void SDLRenderer::clear(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    SDL_RenderClear(renderer);
}

void SDLRenderer::drawRect(SDL_Rect &rect, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    SDL_RenderDrawRect(renderer, &rect);
}

void SDLRenderer::drawTexture(void *texture, const SDL_Rect &src, const SDL_Rect &dest) {
    auto sdlTexture = static_cast<SDL_Texture*>(texture);
    SDL_RenderCopy(renderer, sdlTexture, &src, &dest);
}

unsigned int SDLRenderer::getFlags() const {
    SDL_RendererInfo info;
    SDL_GetRendererInfo(renderer, &info);
    return info.flags;
}
