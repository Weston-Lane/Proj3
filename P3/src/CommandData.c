#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "MacDef.h"
#include "CommandData.h"

char* TokenizeCommand(char* line);

void CommandData_CreateCommand(CommandData* commandData, char* line, int length)
{
    PRINT("Command recieved: [%s][%d]\n", line, length);
    
    bool inToken = false;
    int tStart = 0;
    
    for(int i = 0; i < length + 1; i++)
    {
        
        if(isspace(line[i]) || line[i] == '\0')
        {
            inToken = false;
            //encapsulate token*********************/
            int tokenSize = i - tStart;
            char* token = malloc(sizeof(char) * tokenSize + 1);
            strncpy(token, &line[tStart], tokenSize);
            token[tokenSize] = '\0';
            /************************************/
            
            List_Append(commandData->arguments, token);
            PRINT("Token [%s]\n",token);           
        }
        else if(!inToken)
        {
            inToken = true;
            tStart = i;
        }

    }
    
}

void Print_Command(CommandData* commandData)
{
    
    List_Print(commandData->arguments);
    if(commandData->inFile != NULL)
        { PRINT("InFile: [%s]\n",commandData->inFile); }
    if(commandData->outFile != NULL)
        { PRINT("OutFile: [%s]\n",commandData->outFile); }
}



