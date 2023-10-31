#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

#include "../include/defines.h"
#include "../include/render.h"
#include "../include/game.h"

#define FONT_SIZE 49
#define FRAME_DELAY 15

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Rect backgroundPanel, mainTitleRect, startButtonRect, closeButtonRect, settingsButtonRect, settingsExitButtonRect;
TTF_Font* font = NULL;
SDL_Color textColor = {255, 255, 255, 255};

Button startButton;
Button closeButton;
Button settingsButton;
Button settingsExitButton;

int main(int argc, char* argv[]) {
	int currentMenuState = MAIN_MENU;
    int returnValue = SUCCESS;
    AssetDefinitions assets = {
        {
            "assets/images/textures/backgrounds/background.png"
        },
        {
            "assets/audio/background/track0.mp3",
            "assets/audio/sfx/buttonClick.mp3"
        }
    };

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        returnValue = VIDEO_INIT_FAIL;
        goto exitSequence;
	}

    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
        returnValue = IMG_INIT_FAIL;
        goto exitSequence;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        returnValue = MIXER_INIT_FAIL;
        goto exitSequence;
    }

    Mix_Music* backgroundMusic = NULL;
    Mix_Chunk* buttonClickSound = NULL;

    SDL_DisplayMode dm;
    SDL_GetDesktopDisplayMode(0, &dm);

    window = SDL_CreateWindow("Sneezy Ninja", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, dm.w, dm.h, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        returnValue = WINDOW_CREATE_FAIL;
        goto exitSequence;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        returnValue = RENDERER_FAIL;
        goto exitSequence;
    }
	
	if (TTF_Init() == -1) {
        returnValue = TTF_INIT_FAIL;
        goto exitSequence;
    }

    font = TTF_OpenFont("assets/fonts/rubik.ttf", FONT_SIZE);

    if (font == NULL) {
        returnValue = FONT_LOAD_FAIL;
        goto exitSequence;
    }

    int centerX = dm.w / 2;
    int centerY = dm.h / 2;
    int buttonWidth = 200;
    int buttonHeight = 66;
    int buttonSpacing = 20;
	int titleWidth = dm.w / 2;
	int titleHeight = dm.h / 4;
	
	backgroundPanel.x = 0;
	backgroundPanel.y = 0;
	backgroundPanel.w = dm.w;
	backgroundPanel.h = dm.h;

    startButtonRect.x = centerX - buttonWidth / 2;
    startButtonRect.y = centerY - buttonHeight - buttonSpacing;
    startButtonRect.w = buttonWidth;
    startButtonRect.h = buttonHeight;

    settingsButtonRect.x = centerX - buttonWidth / 2;
    settingsButtonRect.y = centerY;
    settingsButtonRect.w = buttonWidth;
    settingsButtonRect.h = buttonHeight;

    closeButtonRect.x = centerX - buttonWidth / 2;
    closeButtonRect.y = centerY + buttonHeight + buttonSpacing;
    closeButtonRect.w = buttonWidth;
    closeButtonRect.h = buttonHeight;

    settingsExitButtonRect.x = centerX - buttonWidth / 2;
    settingsExitButtonRect.y = centerY + (buttonHeight + buttonSpacing)*2;
    settingsExitButtonRect.w = buttonWidth;
    settingsExitButtonRect.h = buttonHeight;
	
	mainTitleRect.x = centerX - titleWidth / 2;
    mainTitleRect.y = centerY - (titleHeight * 3) / 2;
    mainTitleRect.w = centerX;
    mainTitleRect.h = centerY / 2;
	
    startButton.rect = startButtonRect;
    startButton.bgColor = (SDL_Color){0, 0, 0, 255};
    startButton.bgColorHover = (SDL_Color){20, 20, 20, 255};
    startButton.isHovered = 0;

    closeButton.rect = closeButtonRect;
    closeButton.bgColor = (SDL_Color){0, 0, 0, 255};
    closeButton.bgColorHover = (SDL_Color){20, 20, 20, 255};
    closeButton.isHovered = 0;

    settingsButton.rect = settingsButtonRect;
    settingsButton.bgColor = (SDL_Color){0, 0, 0, 255};
    settingsButton.bgColorHover = (SDL_Color){20, 20, 20, 255};
    settingsButton.isHovered = 0;
	
	settingsExitButton.rect = settingsExitButtonRect;
    settingsExitButton.bgColor = (SDL_Color){0, 0, 0, 255};
    settingsExitButton.bgColorHover = (SDL_Color){20, 20, 20, 255};
    settingsExitButton.isHovered = 0;
	

    SDL_Texture* backgroundTexture = IMG_LoadTexture(renderer, assets.imageDefinitions[0]);

    if (backgroundTexture == NULL) {
        returnValue = IMAGE_FAIL;
        goto exitSequence;
    }

    backgroundMusic = Mix_LoadMUS(assets.audioDefinitions[0]);
    buttonClickSound = Mix_LoadWAV(assets.audioDefinitions[1]);

    if (buttonClickSound == NULL || backgroundMusic == NULL) {
        returnValue = AUDIO_LOAD_FAIL;
        goto exitSequence;
    }

    int screenNeedsUpdate = 1;
    Mix_PlayMusic(backgroundMusic, -1);

    SDL_Event event;
    bool quit = false;
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            } else if (event.type == SDL_MOUSEMOTION) {
				if (currentMenuState == MAIN_MENU) {
					int mouseX = event.motion.x;
					int mouseY = event.motion.y;

					startButton.isHovered 		= (mouseX >= startButtonRect.x && mouseX <= startButtonRect.x + startButtonRect.w && mouseY >= startButtonRect.y && mouseY <= startButtonRect.y + startButtonRect.h);
					closeButton.isHovered 		= (mouseX >= closeButtonRect.x && mouseX <= closeButtonRect.x + closeButtonRect.w && mouseY >= closeButtonRect.y && mouseY <= closeButtonRect.y + closeButtonRect.h);
					settingsButton.isHovered	= (mouseX >= settingsButtonRect.x && mouseX <= settingsButtonRect.x + settingsButtonRect.w && mouseY >= settingsButtonRect.y && mouseY <= settingsButtonRect.y + settingsButtonRect.h);

					screenNeedsUpdate = 1;
				} else if (currentMenuState == SETTINGS_MENU) {
					int mouseX = event.motion.x;
					int mouseY = event.motion.y;

					settingsExitButton.isHovered 		= (mouseX >= settingsExitButtonRect.x && mouseX <= settingsExitButtonRect.x + settingsExitButtonRect.w && mouseY >= settingsExitButtonRect.y && mouseY <= settingsExitButtonRect.y + settingsExitButtonRect.h);

					screenNeedsUpdate = 1;
				}
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
				if (currentMenuState == MAIN) {
					if (startButton.isHovered) {
						screenNeedsUpdate = 1;
						Mix_PlayChannel(-1, buttonClickSound, 0);
						startGame();
					} else if (settingsButton.isHovered) {
						screenNeedsUpdate = 1;
						Mix_PlayChannel(-1, buttonClickSound, 0);
                        currentMenuState = SETTINGS_MENU;
					} else if (closeButton.isHovered) {
						returnValue = SUCCESS;
						goto exitSequence;
					}
				} else if (currentMenuState == SETTINGS_MENU) {
					if (settingsExitButton.isHovered) {
						screenNeedsUpdate = 1;
						Mix_PlayChannel(-1, buttonClickSound, 0);
                        currentMenuState = MAIN;
					}
				}
            }
        }

        if (screenNeedsUpdate) {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
			SDL_RenderCopy(renderer, backgroundTexture, NULL, &backgroundPanel);

            if (currentMenuState == MAIN) {
				if (startButton.isHovered) {
					renderRoundedRect(renderer, startButton.rect, 10, startButton.bgColorHover);
					renderText("Start", startButton.rect);
				} else {
					renderRoundedRect(renderer, startButton.rect, 10, startButton.bgColor);
					renderText("Start", startButton.rect);
				}

				if (closeButton.isHovered) {
					renderRoundedRect(renderer, closeButton.rect, 10, closeButton.bgColorHover);
					renderText("Close", closeButton.rect);
				} else {
					renderRoundedRect(renderer, closeButton.rect, 10, closeButton.bgColor);
					renderText("Close", closeButton.rect);
				}
				
				if (settingsButton.isHovered) {
					renderRoundedRect(renderer, settingsButton.rect, 10, settingsButton.bgColorHover);
					renderText("Settings", settingsButton.rect);
				} else {
					renderRoundedRect(renderer, settingsButton.rect, 10, settingsButton.bgColor);
					renderText("Settings", settingsButton.rect);
				}
			} else if (currentMenuState == SETTINGS_MENU) {
				if (settingsExitButton.isHovered) {
					renderRoundedRect(renderer, settingsExitButton.rect, 10, settingsExitButton.bgColorHover);
					renderText("Menu", settingsExitButton.rect);
				} else {
					renderRoundedRect(renderer, settingsExitButton.rect, 10, settingsExitButton.bgColor);
					renderText("Menu", settingsExitButton.rect);
				}
			}
			
			renderText("Sneezy Ninja", mainTitleRect);

            SDL_RenderPresent(renderer);
            screenNeedsUpdate = 0;
        } else {
            SDL_Delay(FRAME_DELAY);
        }
    }

exitSequence:
    Mix_FreeMusic(backgroundMusic);
    Mix_FreeChunk(buttonClickSound);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(font);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    return returnValue;
}