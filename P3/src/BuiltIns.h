#include "CommandData.h"

#define SH_EXIT "exit"
#define SH_DIE  "die"
#define SH_WHICH "which"
#define SH_PWD  "pwd"
#define SH_CD   "cd"

int HandleDie(CommandData commands[]);
int HandleCD(CommandData commands[]);
int HandleWhich(char* args[]);
int HandlePWD();
char* FindFullPath(char* command);
void FreeFullPath(char* path_to_free, char* command);