#ifndef FILES_H
#define FILES_H

#include "defines.h"

struct MainCharacterStats mainChar;

/*
This function takes the MainCharacter struct defined in defines.h,
and uses JSON-C to save the sfxVol and musicVol values to a file stored in text/savefile.json.
*/
int saveGame(struct MainCharacterStats mainChar);

/*
This function takes a pointer to the struct MainCharacter and loads
sfxVol and musicVol from the text/savefile.json file.
*/
int loadGame(struct MainCharacterStats *mainChar);

#endif
