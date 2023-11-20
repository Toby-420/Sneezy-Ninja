#ifndef GAME_H
#define GAME_H

#include "defines.h"

/*
This starts a new game (TBD)
*/

int startGame(void);

/*
This loads the save game data from text/savefile.json (TBD)
and starts the game with it
*/

int loadPreviousGame(CharacterStats *mainChar);

#endif