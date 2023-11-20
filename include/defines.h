#ifndef DEFINES_H
#define DEFINES_H

#include <stdbool.h>
#include <SDL2/SDL.h>

#define FONT_SIZE 49
#define FRAME_DELAY 15
#define MAIN_CHAR_MAX_HEALTH 100
#define MAIN_CHAR_MAX_SPEED 10
#define MAX_ITEMS 128

/*
Some enums of function error return codes
*/

typedef enum { SUCCESS, VIDEO_INIT_FAIL, TTF_INIT_FAIL, IMG_INIT_FAIL, MIXER_INIT_FAIL, WINDOW_CREATE_FAIL, RENDERER_FAIL, FONT_LOAD_FAIL, IMAGE_FAIL, AUDIO_LOAD_FAIL} ERROR_RETURN_CODES;

/*
What the screen is displaying
*/

typedef enum { MAIN_MENU, SETTINGS_MENU, START_SCREEN, GAME_SCREEN, INVENTORY } MenuState;

/*
Different assets loaded in from files go in these
*/

typedef struct {
	const char imageDefinitions[2][50];
	const char audioDefinitions[8][45];
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
*/

typedef struct {
	unsigned int health;
	uint8_t speed;
} CharacterStats;

/*
Structs for the inventory system
*/

typedef struct {
	int itemID;
	char name[64];
	int quantity;
	int price;
} Item;

typedef struct {
	Item items[MAX_ITEMS];
	int numItems;
} Inventory;

#endif