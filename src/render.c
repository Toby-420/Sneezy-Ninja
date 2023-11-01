#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include "../include/defines.h"

extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern SDL_Rect backgroundPanel, mainTitleRect, startButtonRect, closeButtonRect, settingsButtonRect;
extern SDL_Rect settingsExitButtonRect, sfxVolumeButtonRect, musicVolumeButtonRect, infoBoxRect;
extern SDL_Rect newGameButtonRect, loadGameButtonRect, backButtonRect;
extern TTF_Font* font;
extern SDL_Color textColor;

extern Button startButton;
extern Button closeButton;
extern Button settingsButton;
extern Button settingsExitButton;
extern Button sfxVolumeButton;
extern Button musicVolumeButton;
extern Button newGameButton;
extern Button loadGameButton;
extern Button backButton;
extern Box infoBox;

int renderText(const char* text, SDL_Rect destRect) {
    SDL_Surface* textSurface = TTF_RenderText_Blended(font, text, textColor);
    if (textSurface == NULL) {
        return 1;
    }

    // Get the dimensions of the rendered text
    int textWidth = textSurface->w;
    int textHeight = textSurface->h;

    // Calculate the position to center the text within the destination rectangle
    int centerX = destRect.x + (destRect.w - textWidth) / 2;
    int centerY = destRect.y + (destRect.h - textHeight) / 2;

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (textTexture == NULL) {
        return 2;
    }

    SDL_Rect textDestRect = { centerX, centerY, textWidth, textHeight };
    SDL_RenderCopy(renderer, textTexture, NULL, &textDestRect);
    SDL_DestroyTexture(textTexture);
    
    SDL_FreeSurface(textSurface);
    
    return 0;
}

void SDL_RenderFillCircle(SDL_Renderer* renderer, int x, int y, int radius, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    int radiusSquared = radius * radius;

    for (int h = -radius; h <= radius; h++) {
        for (int w = -radius; w <= radius; w++) {
            int distanceSquared = (w * w) + (h * h);
            if (distanceSquared <= radiusSquared) {
                SDL_RenderDrawPoint(renderer, x + w, y + h);
            }
        }
    }
}

void renderRoundedRect(SDL_Renderer* renderer, SDL_Rect rect, int radius, SDL_Color color) {
    // Render the filled part of the rounded rectangle
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    // Draw the four filled rectangles for the sides
    SDL_Rect topRect = {rect.x + radius, rect.y, rect.w - 2 * radius, radius};
    SDL_Rect bottomRect = {rect.x + radius, rect.y + rect.h - radius, rect.w - 2 * radius, radius};
    SDL_Rect leftRect = {rect.x, rect.y + radius, radius, rect.h - 2 * radius};
    SDL_Rect rightRect = {rect.x + rect.w - radius, rect.y + radius, radius, rect.h - 2 * radius};
	SDL_Rect roundedRect = {
        rect.x + radius,
        rect.y,
        rect.w - 2 * radius,
        rect.h
    };

    SDL_RenderFillRect(renderer, &topRect);
    SDL_RenderFillRect(renderer, &bottomRect);
    SDL_RenderFillRect(renderer, &leftRect);
    SDL_RenderFillRect(renderer, &rightRect);
	SDL_RenderFillRect(renderer, &roundedRect);

    // Draw the four circles at the corners
    int centerX = rect.x + radius;
    int centerY = rect.y + radius;

    SDL_RenderFillCircle(renderer, centerX, centerY, radius, color);
    SDL_RenderFillCircle(renderer, centerX + rect.w - 2 * radius, centerY, radius, color);
    SDL_RenderFillCircle(renderer, centerX, centerY + rect.h - 2 * radius, radius, color);
    SDL_RenderFillCircle(renderer, centerX + rect.w - 2 * radius, centerY + rect.h - 2 * radius, radius, color);
}

int renderTextPositional(const char* text, SDL_Rect destRect, int textYPosition) {
    SDL_Surface* textSurface = TTF_RenderText_Blended(font, text, textColor);
    if (textSurface == NULL) {
        return 1;
    }

    // Get the dimensions of the rendered text
    int textWidth = textSurface->w;
    int textHeight = textSurface->h;

    // Calculate the position to center the text within the destination rectangle
    int centerX = destRect.x + (destRect.w - textWidth) / 2;
    int centerY = destRect.y + textYPosition;

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (textTexture == NULL) {
        return 2;
    }

    SDL_Rect textDestRect = { centerX, centerY, textWidth, textHeight };
    SDL_RenderCopy(renderer, textTexture, NULL, &textDestRect);
    SDL_DestroyTexture(textTexture);
    
    SDL_FreeSurface(textSurface);
    
    return 0;
}