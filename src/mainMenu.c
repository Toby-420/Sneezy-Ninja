#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <json-c/json.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "../include/defines.h"
#include "../include/files.h"
#include "../include/game.h"
#include "../include/render.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Rect backgroundPanel, mainTitleRect, startButtonRect, closeButtonRect, settingsButtonRect;
SDL_Rect settingsExitButtonRect, sfxVolumeButtonRect, musicVolumeButtonRect, infoBoxRect, loadingBoxRect;
SDL_Rect newGameButtonRect, loadGameButtonRect, backButtonRect;
TTF_Font* font = NULL;
SDL_Color textColor = {255, 255, 255, 255};

Button startButton;
Button closeButton;
Button settingsButton;
Button settingsExitButton;
Button sfxVolumeButton;
Button musicVolumeButton;
Button newGameButton;
Button loadGameButton;
Button backButton;
Box loadingBox;
Box infoBox;

int sfxVol = MIX_MAX_VOLUME;
int musicVol = MIX_MAX_VOLUME;
bool sfxDecHovered, sfxIncHovered, musicDecHovered, musicIncHovered;
	
AssetDefinitions assets = {
	{
		"assets/images/textures/backgrounds/background.png"
	},
	{
		"assets/audio/background/track0.mp3",
		"assets/audio/background/gandalf.mp3",
		"assets/audio/sfx/buttonClick.mp3",
		"assets/audio/sfx/errorTypeOne.mp3",
		"assets/audio/sfx/errorTypeTwo.mp3",
		"assets/audio/sfx/epicGame.mp3",
		"assets/audio/sfx/coryNonce.mp3",
	}
};

int main(int argc, char* argv[]) {
	FILE *logFile = fopen("text\\log", "w");
  int currentMenuState = MAIN_MENU;
  int returnValue = SUCCESS;
	char errorMeaning[64] = "\0";
	
	CharacterStats mainChar;
	mainChar.health = MAIN_CHAR_MAX_HEALTH;
	mainChar.speed = MAIN_CHAR_MAX_SPEED;
	
	strcpy(errorMeaning, "Nothing needs to be done! All files are valid :)");
	
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    returnValue = VIDEO_INIT_FAIL;
		strcpy(errorMeaning, "SDL Video failed to initialise\nThere is not much you can do other than redownload the working game :)");
    goto exitSequence_Quit;
  }

  if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
    returnValue = IMG_INIT_FAIL;
		strcpy(errorMeaning, "SDL Image failed to initialise\nThere is not much you can do other than redownload the working game :)");
    goto exitSequence_Quit;
  }

  if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
    returnValue = MIXER_INIT_FAIL;
		strcpy(errorMeaning, "SDL Mixer failed to initialise\nThere is not much you can do other than redownload the working game :)");
    goto exitSequence_Quit;
  }

  Mix_Music* backgroundMusic = NULL;
  Mix_Music* gandalfSoundTrack = NULL;
  Mix_Chunk* buttonClickSound = NULL;
  Mix_Chunk* errorTypeOne = NULL;
  Mix_Chunk* errorTypeTwo = NULL;
  Mix_Chunk* epicGame = NULL;
  Mix_Chunk* coryNonce = NULL;

  SDL_DisplayMode dm;
  SDL_GetDesktopDisplayMode(0, &dm);
  if (dm.h != (9*dm.w)/16) {
		window = SDL_CreateWindow("Sneezy Ninja", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, dm.w, dm.h, SDL_WINDOW_FULLSCREEN);
	} // change this to resize for aspects other than 16:9

  window = SDL_CreateWindow("Sneezy Ninja", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, dm.w, dm.h, SDL_WINDOW_SHOWN);
  if (window == NULL) {
    returnValue = WINDOW_CREATE_FAIL;
		strcpy(errorMeaning, "SDL failed to create a window\nResizing or changing the default resolution might help. Not sure though :)");
    goto exitSequence_Quit;
  }

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (renderer == NULL) {
    returnValue = RENDERER_FAIL;
		strcpy(errorMeaning, "SDL failed to create a renderer\nThere is not much you can do other than redownload the working game :)");
    goto exitSequence_Quit;
  }

  if (TTF_Init() == -1) {
    returnValue = TTF_INIT_FAIL;
		strcpy(errorMeaning, "SDL TTF (True Type Font) failed to initialise\nThere is not much you can do other than redownload the working game :)");
    goto exitSequence_Quit;
  }

  font = TTF_OpenFont("assets/fonts/rubik.ttf", FONT_SIZE);

  if (font == NULL) {
    returnValue = FONT_LOAD_FAIL;
		strcpy(errorMeaning, "SDL failed to load the font\nMake sure rubik.ttf is in GAME_FOLDER/assets/fonts/ and it is valid :)");
    goto exitSequence_Font;
  }

  int centerX = dm.w / 2;
  int centerY = dm.h / 2;
  int buttonWidth = 200;
  int buttonHeight = 66;
  int buttonSpacing = 20;
  int titleWidth = dm.w / 2;
  int titleHeight = dm.h / 4;

  int errorCount = 0;

  char buffer[5];

  bool gamePreviouslySaved;

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
  settingsExitButtonRect.y = centerY + (buttonHeight + buttonSpacing) * 2;
  settingsExitButtonRect.w = buttonWidth;
  settingsExitButtonRect.h = buttonHeight;

  musicVolumeButtonRect.x = centerX - buttonWidth / 2;
  musicVolumeButtonRect.y = centerY - buttonHeight - buttonSpacing;
  musicVolumeButtonRect.w = buttonWidth;
  musicVolumeButtonRect.h = buttonHeight;

  sfxVolumeButtonRect.x = centerX - buttonWidth / 2;
  sfxVolumeButtonRect.y = centerY;
  sfxVolumeButtonRect.w = buttonWidth;
  sfxVolumeButtonRect.h = buttonHeight;

  newGameButtonRect.x = centerX - buttonWidth / 2;
  newGameButtonRect.y = centerY - buttonHeight - buttonSpacing;
  newGameButtonRect.w = buttonWidth;
  newGameButtonRect.h = buttonHeight;

  loadGameButtonRect.x = centerX - buttonWidth / 2;
  loadGameButtonRect.y = centerY;
  loadGameButtonRect.w = buttonWidth;
  loadGameButtonRect.h = buttonHeight;

  backButtonRect.x = centerX - buttonWidth / 2;
  backButtonRect.y = centerY + (buttonHeight + buttonSpacing) * 2;
  backButtonRect.w = buttonWidth;
  backButtonRect.h = buttonHeight;

  infoBoxRect.x = centerX + buttonWidth;
  infoBoxRect.y = centerY - buttonHeight - buttonSpacing;
  infoBoxRect.w = buttonWidth * 2;
  infoBoxRect.h = titleHeight;
	
	loadingBoxRect.x = centerX + buttonWidth;
  loadingBoxRect.y = centerY - buttonHeight - buttonSpacing;
  loadingBoxRect.w = buttonWidth * 2;
  loadingBoxRect.h = titleHeight;

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

  musicVolumeButton.rect = musicVolumeButtonRect;
  musicVolumeButton.bgColor = (SDL_Color){0, 0, 0, 255};
  musicVolumeButton.bgColorHover = (SDL_Color){20, 20, 20, 255};
  musicVolumeButton.isHovered = 0;

  sfxVolumeButton.rect = sfxVolumeButtonRect;
  sfxVolumeButton.bgColor = (SDL_Color){0, 0, 0, 255};
  sfxVolumeButton.bgColorHover = (SDL_Color){20, 20, 20, 255};
  sfxVolumeButton.isHovered = 0;

  newGameButton.rect = newGameButtonRect;
  newGameButton.bgColor = (SDL_Color){0, 0, 0, 255};
  newGameButton.bgColorHover = (SDL_Color){20, 20, 20, 255};
  newGameButton.isHovered = 0;

  loadGameButton.rect = loadGameButtonRect;
  loadGameButton.bgColor = (SDL_Color){0, 0, 0, 255};
  loadGameButton.bgColorHover = (SDL_Color){20, 20, 20, 255};
  loadGameButton.isHovered = 0;

  backButton.rect = backButtonRect;
  backButton.bgColor = (SDL_Color){0, 0, 0, 255};
  backButton.bgColorHover = (SDL_Color){20, 20, 20, 255};
  backButton.isHovered = 0;

  infoBox.rect = infoBoxRect;
  infoBox.bgColor = (SDL_Color){10, 10, 10, 255};
	
	loadingBox.rect = loadingBoxRect;
  loadingBox.bgColor = (SDL_Color){10, 10, 10, 255};


	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
  renderText("Loading...", loadingBox.rect);


	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

  SDL_Texture* backgroundTexture = IMG_LoadTexture(renderer, assets.imageDefinitions[0]);

  FILE* saveFileRead = fopen("text\\savefile.json", "r");
  if (saveFileRead == NULL) {
    gamePreviouslySaved = false;
  } else {
    gamePreviouslySaved = true;
  }

  fclose(saveFileRead);

  if (backgroundTexture == NULL) {
    returnValue = IMAGE_FAIL;
		strcpy(errorMeaning, "SDL failed to load the background image\nMake sure background.png GAME_FOLDER/assets/images/ and it is valid :)");
    goto exitSequence_Image;
  }

  backgroundMusic 	= Mix_LoadMUS(assets.audioDefinitions[0]);
  gandalfSoundTrack = Mix_LoadMUS(assets.audioDefinitions[1]);
  buttonClickSound 	= Mix_LoadWAV(assets.audioDefinitions[2]);
  errorTypeOne 			= Mix_LoadWAV(assets.audioDefinitions[3]);
  errorTypeTwo 			= Mix_LoadWAV(assets.audioDefinitions[4]);
  epicGame 					= Mix_LoadWAV(assets.audioDefinitions[5]);
  coryNonce 				= Mix_LoadWAV(assets.audioDefinitions[6]);

  if (buttonClickSound == NULL || gandalfSoundTrack == NULL || backgroundMusic == NULL || errorTypeOne == NULL || errorTypeTwo == NULL || epicGame == NULL || coryNonce == NULL) {
    returnValue = AUDIO_LOAD_FAIL;
		strcpy(errorMeaning, "SDL failed to load at least one audio\nMake sure all audio files are in GAME_FOLDER/assets/audio/PATH and are valid :)");
    goto exitSequence_Music;
  }

  int screenNeedsUpdate = 1;
  Mix_PlayMusic(backgroundMusic, -1);

  SDL_Event event;
  bool quit = false;
  const int konamiCode[] = {SDLK_UP, SDLK_UP, SDLK_DOWN, SDLK_DOWN, SDLK_LEFT, SDLK_RIGHT, SDLK_LEFT, SDLK_RIGHT, SDLK_b, SDLK_a};
	const int coryNonceCode[] = {SDLK_c, SDLK_o, SDLK_r, SDLK_y, SDLK_SPACE, SDLK_n, SDLK_o, SDLK_n, SDLK_c, SDLK_e};
  int konamiIndex = 0;  // Keeps track of the currently expected key
  bool konamiMode = false;
	int coryNonceIndex = 0;  // Keeps track of the currently expected key
  while (!quit) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        quit = 1;
      } else if (event.type == SDL_KEYDOWN) {
        // Check if the current key in the Konami code matches the pressed key
        if (event.key.keysym.sym == konamiCode[konamiIndex]) {
          konamiIndex++;  // Move to the next expected key
          if (konamiIndex == sizeof(konamiCode) / sizeof(int)) {
            if (!konamiMode) {
              Mix_PlayChannel(-1, epicGame, 0);
              Mix_PlayMusic(gandalfSoundTrack, -1);
              konamiMode = true;
            } else {
              Mix_PlayChannel(-1, epicGame, 0);
              Mix_PlayMusic(backgroundMusic, -1);
              konamiMode = false;
            }

            konamiIndex = 0;
          }
        } else if (event.key.keysym.sym == coryNonceCode[coryNonceIndex]) {
          coryNonceIndex++;  // Move to the next expected key
          if (coryNonceIndex == sizeof(coryNonceCode) / sizeof(int)) {
            Mix_PlayChannel(-1, coryNonce, 0);
          }
        } else {
          // If the pressed key doesn't match the expected one, reset the index
          coryNonceIndex = 0;
          konamiIndex = 0;
        }
      } else if (event.type == SDL_MOUSEMOTION) {
        if (currentMenuState == MAIN_MENU) {
          int mouseX = event.motion.x;
          int mouseY = event.motion.y;

          startButton.isHovered = (mouseX >= startButtonRect.x && mouseX <= startButtonRect.x + startButtonRect.w && mouseY >= startButtonRect.y && mouseY <= startButtonRect.y + startButtonRect.h);
          closeButton.isHovered = (mouseX >= closeButtonRect.x && mouseX <= closeButtonRect.x + closeButtonRect.w && mouseY >= closeButtonRect.y && mouseY <= closeButtonRect.y + closeButtonRect.h);
          settingsButton.isHovered = (mouseX >= settingsButtonRect.x && mouseX <= settingsButtonRect.x + settingsButtonRect.w && mouseY >= settingsButtonRect.y && mouseY <= settingsButtonRect.y + settingsButtonRect.h);

          screenNeedsUpdate = 1;
        } else if (currentMenuState == SETTINGS_MENU) {
          int mouseX = event.motion.x;
          int mouseY = event.motion.y;

          settingsExitButton.isHovered = (mouseX >= settingsExitButtonRect.x && mouseX <= settingsExitButtonRect.x + settingsExitButtonRect.w && mouseY >= settingsExitButtonRect.y && mouseY <= settingsExitButtonRect.y + settingsExitButtonRect.h);
          musicVolumeButton.isHovered = (mouseX >= musicVolumeButtonRect.x && mouseX <= musicVolumeButtonRect.x + musicVolumeButtonRect.w && mouseY >= musicVolumeButtonRect.y && mouseY <= musicVolumeButtonRect.y + musicVolumeButtonRect.h);
          sfxVolumeButton.isHovered = (mouseX >= sfxVolumeButtonRect.x && mouseX <= sfxVolumeButtonRect.x + sfxVolumeButtonRect.w && mouseY >= sfxVolumeButtonRect.y && mouseY <= sfxVolumeButtonRect.y + sfxVolumeButtonRect.h);

          musicIncHovered = (mouseX >= musicVolumeButtonRect.x && mouseX <= musicVolumeButtonRect.x + (musicVolumeButtonRect.w / 2) && mouseY >= musicVolumeButtonRect.y && mouseY <= musicVolumeButtonRect.y + musicVolumeButtonRect.h);
          sfxIncHovered = (mouseX >= sfxVolumeButtonRect.x && mouseX <= sfxVolumeButtonRect.x + (sfxVolumeButtonRect.w / 2) && mouseY >= sfxVolumeButtonRect.y && mouseY <= sfxVolumeButtonRect.y + sfxVolumeButtonRect.h);

          if (!musicIncHovered && musicVolumeButton.isHovered) {
            musicDecHovered = true;
          } else {
            musicDecHovered = false;
          }
          if (!sfxIncHovered && sfxVolumeButton.isHovered) {
            sfxDecHovered = true;
          } else {
            sfxDecHovered = false;
          }

          screenNeedsUpdate = 1;
        } else if (currentMenuState == START_SCREEN) {
          int mouseX = event.motion.x;
          int mouseY = event.motion.y;

          newGameButton.isHovered = (mouseX >= newGameButtonRect.x && mouseX <= newGameButtonRect.x + newGameButtonRect.w && mouseY >= newGameButtonRect.y && mouseY <= newGameButtonRect.y + newGameButtonRect.h);
          loadGameButton.isHovered = (mouseX >= loadGameButtonRect.x && mouseX <= loadGameButtonRect.x + loadGameButtonRect.w && mouseY >= loadGameButtonRect.y && mouseY <= loadGameButtonRect.y + loadGameButtonRect.h);
          backButton.isHovered = (mouseX >= backButtonRect.x && mouseX <= backButtonRect.x + backButtonRect.w && mouseY >= backButtonRect.y && mouseY <= backButtonRect.y + backButtonRect.h);

          screenNeedsUpdate = 1;
        }
      } else if (event.type == SDL_MOUSEBUTTONDOWN) {
        if (currentMenuState == MAIN_MENU) {
          if (startButton.isHovered) {
            screenNeedsUpdate = 1;
            Mix_PlayChannel(-1, buttonClickSound, 0);
            currentMenuState = START_SCREEN;
            screenNeedsUpdate = 1;
          } else if (settingsButton.isHovered) {
            screenNeedsUpdate = 1;
            Mix_PlayChannel(-1, buttonClickSound, 0);
            currentMenuState = SETTINGS_MENU;
            screenNeedsUpdate = 1;
          } else if (closeButton.isHovered) {
            returnValue = SUCCESS;
            goto exitSequence_All;
          }
        } else if (currentMenuState == SETTINGS_MENU) {
          if (settingsExitButton.isHovered) {
            screenNeedsUpdate = 1;
            currentMenuState = MAIN_MENU;
          }
          if (musicIncHovered) {
            // Increase music volume
            if (musicVol <= MIX_MAX_VOLUME - 9) {
              musicVol += 8;
              Mix_VolumeMusic(musicVol);
            } else {
              musicVol = 128;
            }
          } else if (musicDecHovered) {
            // Decrease music volume
            if (musicVol >= 9) {
              musicVol -= 8;
              Mix_VolumeMusic(musicVol);
            } else {
              musicVol = 0;
            }
          }
          if (sfxIncHovered) {
            // Increase sfx volume
            if (sfxVol <= MIX_MAX_VOLUME - 9) {
              sfxVol += 8;
              Mix_MasterVolume(sfxVol);
            } else {
              sfxVol = 128;
            }
          } else if (sfxDecHovered) {
            // Decrease sfx volume
            if (sfxVol >= 9) {
              sfxVol -= 8;
              Mix_MasterVolume(sfxVol);
            } else {
              sfxVol = 0;
            }
          }
          Mix_PlayChannel(-1, buttonClickSound, 0);
        } else if (currentMenuState == START_SCREEN) {
          if (newGameButton.isHovered) {
            screenNeedsUpdate = 1;
            Mix_PlayChannel(-1, buttonClickSound, 0);
            startGame();
          } else if (loadGameButton.isHovered) {
            screenNeedsUpdate = 1;
            if (gamePreviouslySaved) {
              Mix_PlayChannel(-1, buttonClickSound, 0);
              loadPreviousGame(&mainChar);
            } else {
              if (errorCount < 10) {
                Mix_PlayChannel(-1, errorTypeOne, 0);
                errorCount++;
              } else {
                Mix_PlayChannel(-1, errorTypeTwo, 0);
                errorCount = 0;
              }
            }
          } else if (backButton.isHovered) {
            screenNeedsUpdate = 1;
            Mix_PlayChannel(-1, buttonClickSound, 0);
            currentMenuState = MAIN_MENU;
          }
        }
        screenNeedsUpdate = 1;
      }
    }

    if (screenNeedsUpdate) {
      SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
      SDL_RenderClear(renderer);
      SDL_RenderCopy(renderer, backgroundTexture, NULL, &backgroundPanel);

      if (currentMenuState == MAIN_MENU) {
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
        renderRoundedRect(renderer, infoBox.rect, 10, infoBox.bgColor);
        if (settingsExitButton.isHovered) {
          renderRoundedRect(renderer, settingsExitButton.rect, 10, settingsExitButton.bgColorHover);
          renderText("Menu", settingsExitButton.rect);
          renderRoundedRect(renderer, infoBox.rect, 10, infoBox.bgColor);
          renderText("Exit settings", infoBox.rect);
        } else {
          renderRoundedRect(renderer, settingsExitButton.rect, 10, settingsExitButton.bgColor);
          renderText("Menu", settingsExitButton.rect);
        }
        if (musicIncHovered || musicDecHovered) {
          renderRoundedRect(renderer, musicVolumeButton.rect, 10, musicVolumeButton.bgColorHover);
          renderText("+Music-", musicVolumeButton.rect);
          renderTextPositional("-Music Volume-", infoBox.rect, 10);
          renderTextPositional("Current:", infoBox.rect, 60);
          snprintf(buffer, sizeof(buffer), "%d", musicVol);
          const char* string = buffer;
          renderTextPositional(string, infoBox.rect, 110);
        } else {
          renderRoundedRect(renderer, musicVolumeButton.rect, 10, musicVolumeButton.bgColor);
          renderText("+Music-", musicVolumeButton.rect);
        }
        if (sfxIncHovered || sfxDecHovered) {
          renderRoundedRect(renderer, sfxVolumeButton.rect, 10, sfxVolumeButton.bgColorHover);
          renderText("+ SFX -", sfxVolumeButton.rect);
          renderTextPositional("-SFX Volume-", infoBox.rect, 10);
          renderTextPositional("Current:", infoBox.rect, 60);
          snprintf(buffer, sizeof(buffer), "%d", sfxVol);
          const char* string = buffer;
          renderTextPositional(string, infoBox.rect, 110);
        } else {
          renderRoundedRect(renderer, sfxVolumeButton.rect, 10, sfxVolumeButton.bgColor);
          renderText("+ SFX -", sfxVolumeButton.rect);
        }
      } else if (currentMenuState == START_SCREEN) {
        renderRoundedRect(renderer, infoBox.rect, 10, infoBox.bgColor);
        if (newGameButton.isHovered) {
          renderRoundedRect(renderer, newGameButton.rect, 10, newGameButton.bgColorHover);
          renderText("New", newGameButton.rect);
          renderRoundedRect(renderer, infoBox.rect, 10, infoBox.bgColor);
          renderTextPositional("Start new", infoBox.rect, 10);
          renderTextPositional("game", infoBox.rect, 60);
        } else {
          renderRoundedRect(renderer, newGameButton.rect, 10, newGameButton.bgColor);
          renderText("New", newGameButton.rect);
        }
        if (loadGameButton.isHovered) {
          renderRoundedRect(renderer, loadGameButton.rect, 10, loadGameButton.bgColorHover);
          renderText("Load", loadGameButton.rect);
          renderTextPositional("Load game", infoBox.rect, 10);
          if (gamePreviouslySaved) {
            renderTextPositional("Available", infoBox.rect, 60);
          } else {
            renderTextPositional("Unavailable", infoBox.rect, 60);
          }
        } else {
          renderRoundedRect(renderer, loadGameButton.rect, 10, loadGameButton.bgColor);
          renderText("Load", loadGameButton.rect);
        }
        if (backButton.isHovered) {
          renderRoundedRect(renderer, backButton.rect, 10, backButton.bgColorHover);
          renderText("Menu", backButton.rect);
          renderTextPositional("Back to Main", infoBox.rect, 10);
          renderTextPositional("menu", infoBox.rect, 60);
        } else {
          renderRoundedRect(renderer, backButton.rect, 10, backButton.bgColor);
          renderText("Menu", backButton.rect);
        }
      }

      renderText("Sneezy Ninja", mainTitleRect);

      SDL_RenderPresent(renderer);
      screenNeedsUpdate = 0;
    } else {
      SDL_Delay(FRAME_DELAY);
    }
  }

exitSequence_All:
exitSequence_Music:
  Mix_FreeMusic(backgroundMusic);
  Mix_FreeMusic(gandalfSoundTrack);
  Mix_FreeChunk(buttonClickSound);
  Mix_FreeChunk(errorTypeOne);
  Mix_FreeChunk(errorTypeTwo);
  Mix_FreeChunk(epicGame);
  Mix_FreeChunk(coryNonce);

exitSequence_Image:
	SDL_DestroyTexture(backgroundTexture);

exitSequence_Font:
  TTF_CloseFont(font);

exitSequence_Quit:
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  TTF_Quit();
  IMG_Quit();
  SDL_Quit();

	fprintf(logFile, "Error code: %d\nMeaning: %s", returnValue, errorMeaning);
  return returnValue;
}