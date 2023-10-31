#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include "../include/defines.h"

extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern SDL_Rect backgroundPanel, mainTitleRect, startButtonRect, closeButtonRect, settingsButtonRect, settingsExitButtonRect;
extern TTF_Font* font;
extern SDL_Color textColor;

extern Button startButton;
extern Button closeButton;
extern Button settingsButton;
extern Button settingsExitButton;

int startGame(void) { return 0; }