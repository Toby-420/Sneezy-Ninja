@echo off
echo Compiling...
gcc src/* assets/images/icons/icon.o -o game.exe -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -ljson-c -mwindows -Wall -Wpedantic -Werror -O3
echo Changing MZ header...
powershell -noprofile -executionpolicy bypass -file ./scripts/executableTextAlteration.ps1
echo Editing files...
echo Done.
