#include <json-c/json.h>
#include <stdio.h>
#include <string.h>
#include "../include/defines.h"

extern int sfxVol;
extern int musicVol;

int saveGame(CharacterStats mainChar) {
  json_object *root = json_object_new_object();
  json_object_object_add(root, "sfxVol", json_object_new_int(sfxVol));
  json_object_object_add(root, "musicVol", json_object_new_int(musicVol));
  json_object_object_add(root, "charSpeed", json_object_new_int(mainChar.speed));
  json_object_object_add(root, "charHealth", json_object_new_int(mainChar.health));

  const char *jsonString = json_object_to_json_string_ext(root, JSON_C_TO_STRING_PRETTY);

  FILE *file = fopen("text\\savefile.json", "w");
  
  if (file == NULL) return 1;
  
  fprintf(file, "%s", jsonString);
  fclose(file);

  json_object_put(root);
  return 0;
}

int loadGame(CharacterStats *mainChar) {
  FILE *file = fopen("text\\savefile.json", "r");
  
  if (file == NULL) return 1;

  fseek(file, 0, SEEK_END);
  long fileSize = ftell(file);
  fseek(file, 0, SEEK_SET);

  char *jsonString = (char *)malloc(fileSize + 1);
  if (jsonString == NULL) {
    fclose(file);
    printf("Memory allocation error!\n");
    return 1;
  }

  fread(jsonString, 1, fileSize, file);
  fclose(file);
  jsonString[fileSize] = '\0';

  json_object *root = json_tokener_parse(jsonString);
  free(jsonString);

  if (root == NULL) {
    printf("Error parsing JSON character!\n");
    return 1;
  }

  sfxVol = json_object_get_int(json_object_object_get(root, "sfxVol"));
  musicVol = json_object_get_int(json_object_object_get(root, "musicVol"));
  mainChar->speed = json_object_get_int(json_object_object_get(root, "speed"));
  mainChar->health = json_object_get_int(json_object_object_get(root, "health"));

  json_object_put(root);
  return 0;
}
