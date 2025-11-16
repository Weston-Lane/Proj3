#include "BuiltIns.h"

int HandleDie(CommandData commands[])
{
    for(int i = 1; commands[0].arguments->data[i] != NULL; i++)
    {
        fprintf(stderr, "%s ", commands[0].arguments->data[i]);
    }
    fprintf(stderr, "\n");
    return 0;
}

int HandleCD(CommandData commands[])
{
    if (commands[0].arguments->data[1] == NULL) 
    {
        fprintf(stderr, "mysh: cd: expected argument\n");
        return 1;
    } 
    else if (chdir(commands[0].arguments->data[1]) != 0) 
    {
        perror("mysh: cd");
        return 1;
    }

    return 0;
}