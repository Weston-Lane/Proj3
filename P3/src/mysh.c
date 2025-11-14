#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <string.h>

#include "MacDef.h"
#include "ArrayList.h"
#include "CommandData.h"

void HandleInteractiveMode();
char* ReadInput(int* numCommands, int* bytes, int fileID);
void HandleBatchMode(const char* file);
bool IsInteractive(int argc);
void CleanUp(CommandData* commandData);

static bool EXIT_ = false;
static bool INTERACT_MODE = false;
int main(int argc, char** argv)
{
    (void)argv;

    INTERACT_MODE = IsInteractive(argc);
    if(INTERACT_MODE)
    {
        printf("Welcome to my terminal!\n");
    }
    while(!EXIT_)
    {

        if(INTERACT_MODE)
        {
           HandleInteractiveMode();
        }
        else
        {
            HandleBatchMode(argv[1]);
        }
           
    }

    
    
}

bool IsInteractive(int argc)
{
    if(argc == 1 )
        { return isatty(STDIN_FILENO); }
    else
        { return false; }
}

void HandleInteractiveMode()
{
    printf("mysh> ");
    fflush(stdout);

    int numCommands = 1;
    int bytes = 0;
    char* line = ReadInput(&numCommands, &bytes, STDIN_FILENO);
    if (line == NULL) 
        { return; } // Handle Ctrl+D or read error
    PRINT("LINE: %s\nBYTES: %d\nNUMCOMM: %d\n", line,bytes,numCommands);

    
    
    CommandData commands[numCommands];
    int currCommand = 0;
    char* delimiters = " |\n"; 
    char* command_string = strtok(line, delimiters); 
    
    while (command_string != NULL)
    {
        
        CommandData command = {NULL, NULL, NULL};
        command.arguments = List_Create(10);
        
        CommandData_CreateCommand(&command, command_string, strlen(command_string));
        
        if (currCommand < numCommands) 
        {
            commands[currCommand] = command;
            currCommand++;
            Print_Command(&command);
        }
        
        command_string = strtok(NULL, delimiters); 
    }
    
}

void HandleBatchMode(const char* file)
{
    EXIT_ = true;
    int fileID = open(file,O_RDONLY);
    if(file < 0)
        { fprintf(stderr, "ERROR: COULD NOT OPEN FILE FOR PARSING: %s", file); return; }
    
    int numCommands = 1;
    int bytes = 0;
    char* line = ReadInput(&numCommands, &bytes, fileID);
    PRINT("Batch process:\n%s", line);
    free(line);
}

char* ReadInput(int* numCommands, int* bytes, int fileID)
{
    char globalBuffer[BUFFER_SIZE];
    int byte = 0;
    byte = read(fileID,globalBuffer,BUFFER_SIZE);
    *bytes = byte;
    for(int i = 0; i < byte; i++)
    {
        if(globalBuffer[i] == '|')
            { (*numCommands)++; }
    }

    int lineSize = 0;
    if(globalBuffer[byte - 1] == '\n')
        { lineSize = byte - 1; }
    else
        { lineSize = byte; }

    char* line = malloc(sizeof(char) * lineSize + 1);
    strncpy(line, globalBuffer, byte);
    line[lineSize] = '\0';

    return line;
}

void CleanUp(CommandData* commandData)
{
    List_Free(commandData->arguments);
}
