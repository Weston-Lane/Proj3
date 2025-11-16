#include "CommandData.h"

#define SH_EXIT "exit"
#define SH_DIE  "die"
#define SH_WHICH "which"
#define SH_PWD  "pwd"
#define SH_CD   "cd"

int HandleDie(CommandData commands[]);
int HandleCD(CommandData commands[]);