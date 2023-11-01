#ifndef RENDER_H
#define RENDER_H

/*
Renders the specified text at the size defined by FONT_SIZE in defines.h
It justifies the text in the center and spaces correctly

destRect is the SDL_Rect that the text is desired to be in
*/

int renderText(const char* text, SDL_Rect destRect);

/*
This is the same as renderText(), but allows for the vertical positioning
of the text to be specified by textYPosition
*/

int renderTextPositional(const char* text, SDL_Rect destRect, int textYPosition);

/*
This makes a circle (very simply) which is not perfect, but does the job
*/

void SDL_RenderFillCircle(SDL_Renderer* renderer, int x, int y, int radius, SDL_Color color);

/*
This uses  SDL_RenderFillCircle to make SDL_Rectangles with rounded corners
Just apply a radius (10 is good default) for the corners
*/

void renderRoundedRect(SDL_Renderer* renderer, SDL_Rect rect, int radius, SDL_Color color);

#endif