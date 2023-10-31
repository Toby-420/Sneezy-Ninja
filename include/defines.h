#ifndef DEFINES_H
#define DEFINES_H

#include <stdbool.h>

typedef enum { SUCCESS, VIDEO_INIT_FAIL, TTF_INIT_FAIL, IMG_INIT_FAIL, MIXER_INIT_FAIL, WINDOW_CREATE_FAIL, RENDERER_FAIL, FONT_LOAD_FAIL, IMAGE_FAIL, AUDIO_LOAD_FAIL} ERROR_RETURN_CODES;
typedef enum { MAIN_MENU, GAME_PAUSE_MENU } SETTINGS_MENU_OPEN_ORIGINS;
typedef enum { MAIN, SETTINGS_MENU} MenuState;

typedef struct AssetDefinitions {
	const char imageDefinitions[7][64];
	const char audioDefinitions[2][64];
} AssetDefinitions;

typedef struct {
    SDL_Rect rect;
    SDL_Color bgColor;
    SDL_Color bgColorHover;
    bool isHovered;
} Button;


#endif