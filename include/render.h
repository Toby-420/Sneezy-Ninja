#ifndef RENDER_H
#define RENDER_H

int renderText(const char* text, SDL_Rect destRect);
void SDL_RenderFillCircle(SDL_Renderer* renderer, int x, int y, int radius, SDL_Color color);
void renderRoundedRect(SDL_Renderer* renderer, SDL_Rect rect, int radius, SDL_Color color);

#endif