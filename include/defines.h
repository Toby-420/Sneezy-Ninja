#ifndef DEFINES_H
#define DEFINES_H

#include <stdbool.h>
#include <SDL2/SDL.h>

#define FONT_SIZE 49
#define FRAME_DELAY 15
#define MAIN_CHAR_MAX_HEALTH 100
#define MAIN_CHAR_MAX_SPEED 10

/*
Some enums of function error return codes
*/

typedef enum { SUCCESS, VIDEO_INIT_FAIL, TTF_INIT_FAIL, IMG_INIT_FAIL, MIXER_INIT_FAIL, WINDOW_CREATE_FAIL, RENDERER_FAIL, FONT_LOAD_FAIL, IMAGE_FAIL, AUDIO_LOAD_FAIL} ERROR_RETURN_CODES;

/*
Where the user came from when opening the settings menu
*/

typedef enum { MAIN_MENU, GAME_PAUSE_MENU } SETTINGS_MENU_OPEN_ORIGINS;

/*
What part of the mene the main menu screen is in
*/

typedef enum { MAIN, SETTINGS_MENU, START_SCREEN } MenuState;

/*
Different assets loaded in from files go in these
*/

typedef struct {
	const char imageDefinitions[1][50];
	const char audioDefinitions[6][35];
} AssetDefinitions;

/*
Buttons have these different variables attatched to them to allow for proper usage
*/

typedef struct {
    SDL_Rect rect;
    SDL_Color bgColor;
    SDL_Color bgColorHover;
    bool isHovered;
} Button;

/*
Boxes (such as the infoBox) don't need hovering variables as they don't get interacted with
therefore they are labelled Box instead of Button
*/

typedef struct {
    SDL_Rect rect;
    SDL_Color bgColor;
} Box;

/*
Variables for the main character
Expect these to grow in number

This type of struct definition is used over the other type as I got lots of errors
with the other type only for this struct
*/

struct MainCharacterStats {
	unsigned int health;
	uint8_t speed;
};

#endif