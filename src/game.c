#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include "../include/defines.h"
#include "../include/files.h"

extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern SDL_Rect backgroundPanel, mainTitleRect, startButtonRect, closeButtonRect, settingsButtonRect, settingsExitButtonRect;
extern TTF_Font* font;
extern SDL_Color textColor;

extern Button startButton;
extern Button closeButton;
extern Button settingsButton;
extern Button settingsExitButton;

extern int sfxVol;
extern int musicVol;

int startGame(void) { return 0; }

int loadPreviousGame(struct MainCharacterStats *mainChar) {
	loadGame(mainChar);
	return 0;
}