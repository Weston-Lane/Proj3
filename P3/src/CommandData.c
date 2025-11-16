#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "MacDef.h"
#include "CommandData.h"

char* TokenizeCommand(char* line);

typedef enum Redirection
{
    NONE, IN, OUT
}Redirection;

char* CopyString(const char* s)
{
    if (s == NULL) {
        return NULL;
    }
    size_t len = strlen(s) + 1;
    char* new_s = malloc(len);

    if (new_s == NULL) {
        return NULL;
    }

    memcpy(new_s, s, len);
    return new_s;
}
void CommandData_CreateCommand(CommandData* commandData, char* line, int length)
{
    PRINT("Command recieved: [%s][%d]\n", line, length);
    
    Redirection state = NONE;
    commandData->inFile = NULL;
    commandData->outFile = NULL;
    int currToken = 0;

    char* ptr = NULL;
    char* delimeters = " \t\n";
    char* token = strtok_r(line, delimeters,&ptr);

    while(token != NULL)
    {

        switch(state)
        {
            case IN:
            {
                if(commandData->inFile != NULL)
                    { free(commandData->inFile); }
                commandData->inFile = CopyString(token);
                state = NONE;
            }break;
            case OUT:
            {
                if(commandData->outFile != NULL)
                    { free(commandData->outFile); }
                commandData->outFile = CopyString(token);
                state = NONE;
            }break;
            case NONE:
            {
                if(strcmp(token, "<") == 0)
                    { state = IN; }
                else if (strcmp(token, ">") == 0)
                    { state = OUT; }
                else
                {
                    List_Append(commandData->arguments, CopyString(token));
                    currToken++;
                }
            }break;
        }

        token = strtok_r(NULL, delimeters,&ptr);
    }
    
    List_Append(commandData->arguments, NULL);
}

void Print_Command(CommandData* commandData)
{
    
    List_Print(commandData->arguments);
    if(commandData->inFile != NULL)
        { PRINT("InFile: [%s]\n",commandData->inFile); }
    if(commandData->outFile != NULL)
        { PRINT("OutFile: [%s]\n",commandData->outFile); }
}

void Free_Command(CommandData* commandData)
{
    List_Free(commandData->arguments);
    free(commandData->inFile);
    free(commandData->outFile);
}



