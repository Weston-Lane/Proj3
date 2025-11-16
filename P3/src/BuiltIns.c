#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "BuiltIns.h"
#include "MacDef.h"

int HandleDie(CommandData commands[])
{
    for(int i = 1; commands[0].arguments->data[i] != NULL; i++)
    {
        printf("%s ", commands[0].arguments->data[i]);
    }
    printf("\n");
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

int HandleWhich(char* args[]) 
{
    if (args[1] == NULL) {
        // "fails if it is given the wrong number of arguments"
        return 1;
    }

    char* cmd = args[1];
    
    if (strcmp(cmd, "cd") == 0 ||
        strcmp(cmd, "pwd") == 0 ||
        strcmp(cmd, "which") == 0 ||
        strcmp(cmd, "exit") == 0 ||
        strcmp(cmd, "die") == 0) 
    {
        return 1; 
    }

    char* fullPath = FindFullPath(cmd);

    if (fullPath != NULL) {
        printf("%s\n", fullPath);
        FreeFullPath(fullPath, cmd);
        return 0;
    }
    
    return 1; // Not found, failure
}

int HandlePWD()
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) 
    {
        printf("%s\n", cwd);
        return 0; 
    } 
    else 
    {
        perror("mysh: pwd");
        return 1;
    }
}

char* FindFullPath(char* command) 
{
    
    if (strchr(command, '/') != NULL) 
    {
        if (access(command, X_OK) == 0) 
            { return command; } 
        else 
            { return NULL; }
    }

    const char* paths[] = {"/usr/local/bin", "/usr/bin", "/bin", NULL};
    
    for (int j = 0; paths[j] != NULL; j++) 
    {
        char testPath[BUFFER_SIZE];
        snprintf(testPath, sizeof(testPath), "%s/%s", paths[j], command);
        PRINT("FULL PATH: %s\n", testPath);
        if (access(testPath, X_OK) == 0) 
        {
            PRINT("FOUND\n");
            return strdup(testPath);
        }
    }

    return NULL;
}


void FreeFullPath(char* path, char* command) 
{
    if (path != command) 
        { free(path); }
}