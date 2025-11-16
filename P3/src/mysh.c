#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <string.h>

#include "MacDef.h"
#include "ArrayList.h"
#include "CommandData.h"
#include "PipeLineExecution.h"
#include "BuiltIns.h"

void HandleInteractiveMode();
void HandleBatchMode(int fileID);
void ExecuteCommandLine(char* line);
bool IsInteractive(int argc);
void CleanUp(CommandData* commandData, int numCommands);

static bool EXIT_ = false;
static bool INTERACT_MODE = false;
static char GLOBAL_BUFFER[BUFFER_SIZE];
static int LINE_LEN = 0;
static int LAST_EXIT_STATUS = -1;

int main(int argc, char** argv)
{
    (void)argv;

    int fileID = 0;
    INTERACT_MODE = IsInteractive(argc);
    if(INTERACT_MODE)
    {
        printf("Welcome to my terminal!\n");
    }
    else
    {
        fileID = open(argv[1],O_RDONLY);
    }

    while(!EXIT_)
    {

        if(INTERACT_MODE)
        {
           HandleInteractiveMode();
        }
        else
        {
            HandleBatchMode(fileID);
        }
           
    }

    if(INTERACT_MODE)
    {
        printf("Goodbye!\n");
    }
    else
    {
        close(fileID);
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

    char* newLinePos = memchr(GLOBAL_BUFFER,'\n',LINE_LEN);

    if(newLinePos == NULL)
    //call read() for the first time or again if line does not have \n
    {
        while(newLinePos == NULL)
        {
            if(LINE_LEN == BUFFER_SIZE)
            {
                fprintf(stderr, "mysh: Command length too long.\n");
                LINE_LEN = 0;
                break;
            }
            
            
            
            int bytesRead = read(STDIN_FILENO, 
                             GLOBAL_BUFFER + LINE_LEN, //append to end
                             BUFFER_SIZE - LINE_LEN); //only read remaining space
            
            if(bytesRead == 0)
            //end of stream reached (ctrlD or EOF)
            {
                if(LINE_LEN > 0)
                //line does not end in \n but will still break and process
                {
                    GLOBAL_BUFFER[LINE_LEN] = '\n';
                    newLinePos = &GLOBAL_BUFFER[LINE_LEN];
                    LINE_LEN++;
                    break;
                }

                printf("\n");
                
                return;
            }

            if(bytesRead < 0)
            {
                perror("[Read Error]");
                return;
            }

            LINE_LEN += bytesRead;
            
            newLinePos = memchr(GLOBAL_BUFFER,'\n',LINE_LEN);
        }
        
    }
    int commandLen = (char*)newLinePos - (char*)GLOBAL_BUFFER;
        
    // Malloc for the command (or use VLA)
    char commandStr[commandLen + 1];
    strncpy(commandStr, GLOBAL_BUFFER, commandLen);
    commandStr[commandLen] = '\0';

    //! IMPLEMENTATION DOES NOT ALLOW FOR COMMMENTS TO BE IN BETWEEN NEWLINES.
    char* commentStart = strchr(commandStr, '#');
    if(commentStart != NULL)
    //If a comment is found, truncates rest of line.
        { *commentStart = '\0'; }

    int leftoverLen = LINE_LEN - (commandLen + 1);
    // Use memmove for overlapping memory
    memmove(GLOBAL_BUFFER, newLinePos + 1, leftoverLen);
    LINE_LEN = leftoverLen;

    PRINT("Command: %s\n",commandStr);
    ExecuteCommandLine(commandStr);
  
}

void HandleBatchMode(int fileID)
{
    
    char* newLinePos = memchr(GLOBAL_BUFFER,'\n',LINE_LEN);

    if(newLinePos == NULL)
    //call read() for the first time or again if line does not have \n
    {
        while(newLinePos == NULL)
        {
            if(LINE_LEN == BUFFER_SIZE)
            {
                fprintf(stderr, "mysh: Command length too long.\n");
                LINE_LEN = 0;
                break;
            }
            
            
            
            int bytesRead = read(fileID, 
                             GLOBAL_BUFFER + LINE_LEN, //append to end
                             BUFFER_SIZE - LINE_LEN); //only read remaining space
            
            if(bytesRead == 0)
            //end of stream reached (ctrlD or EOF)
            {
                if(LINE_LEN > 0)
                //line does not end in \n but will still break and process
                {
                    GLOBAL_BUFFER[LINE_LEN] = '\n';
                    newLinePos = &GLOBAL_BUFFER[LINE_LEN];
                    LINE_LEN++;
                    break;
                }

                printf("\n");
                EXIT_ = true;
                return;
            }

            if(bytesRead < 0)
            {
                perror("[Read Error]");
                return;
            }

            LINE_LEN += bytesRead;
            newLinePos = memchr(GLOBAL_BUFFER,'\n',LINE_LEN);
        }
        
    }

    int commandLen = (char*)newLinePos - (char*)GLOBAL_BUFFER;
        
    // Malloc for the command (or use VLA)
    char commandStr[commandLen + 1];
    strncpy(commandStr, GLOBAL_BUFFER, commandLen);
    commandStr[commandLen] = '\0';

    //! IMPLEMENTATION DOES NOT ALLOW FOR COMMMENTS TO BE IN BETWEEN NEWLINES.
    char* commentStart = strchr(commandStr, '#');
    if(commentStart != NULL)
    //If a comment is found, truncates rest of line.
        { *commentStart = '\0'; }

    int leftoverLen = LINE_LEN - (commandLen + 1);
        
    // Use memmove for overlapping memory
    memmove(GLOBAL_BUFFER, newLinePos + 1, leftoverLen);
    LINE_LEN = leftoverLen;

    if (strcmp(commandStr, "exit") == 0 || 
        strcmp(commandStr, "die") == 0) 
    {
        EXIT_ = true;
        return; 
    }

    PRINT("Command: %s\n",commandStr);
    ExecuteCommandLine(commandStr);
    
}

void ExecuteCommandLine(char* line)
{
    
    /*CHECK LAST CONDITIONALS*/
    bool shouldRun = true;
    if(strncmp(line, SH_AND, 4) == 0)
    {
        if(LAST_EXIT_STATUS == -1)
            { printf("mysh: CANNOT 'AND' BEFORE FIRST OPERATION\n"); return; }
        if(LAST_EXIT_STATUS != 0)
        //fail
            { shouldRun = false; }
        line += 4;
    }
    else if(strncmp(line, SH_OR, 3) == 0)
    {
        if(LAST_EXIT_STATUS == -1)
            { printf("mysh: CANNOT 'OR' BEFORE FIRST OPERATION\n"); return; }
        if(LAST_EXIT_STATUS == 0)
        //pass
            { shouldRun = false; }
        line += 3;
    }

    if(!shouldRun)
        { return; }
    /******************************************/

    int lineLen = strlen(line);
    if(lineLen <= 1)
        { return; }

    int numPipes = 0;
    for(int i = 0; i < lineLen; i++)
    { 
        if(line[i]=='|')
            { numPipes++; }
    }

    int numCommands = numPipes + 1;
    CommandData commands[numCommands];
    PRINT("NUMCOMMANDS: %d\n",numCommands);
    char* ptr = NULL;
    char* delimeters = "|";
    char* cmdSubStr = strtok_r(line,delimeters,&ptr);
    int i = 0;
    while(cmdSubStr != NULL)
    {
        PRINT("substr: %s\n",cmdSubStr);
        if(i>=numCommands)
            { break; }
        
        commands[i].arguments = List_Create(10);
        CommandData_CreateCommand(&commands[i], cmdSubStr, strlen(cmdSubStr));
        i++;
        cmdSubStr = strtok_r(NULL, delimeters, &ptr);
    }

    /*CHECK FOR exit, cd, AND, die COMMANDS*/
    bool isBuiltIn = false;
    char* cmd = commands[0].arguments->data[0];

    if(numCommands == 1 && 
        commands[0].arguments->data[0] != NULL &&
        commands[0].arguments->data[0][0] != '/')
    {
        
        if (strcmp(cmd, "cd") == 0)
        {
            isBuiltIn = true;
            LAST_EXIT_STATUS = HandleCD(commands);
        }
    }
    
    /********************************/

    if(!isBuiltIn)
    {
        LAST_EXIT_STATUS = ExecutePipeLine(commands, numCommands);
        
    }

    cmd = commands[numCommands - 1].arguments->data[0];
    if((strcmp(cmd, SH_EXIT) == 0 || strcmp(cmd, SH_DIE) == 0))
    //check if last arg is either a die or exit command for case : prog | exit/die
    //THIS WILL ONLY WORK IN THE CASE THAT THE LAST ARG IS die OR exit OTHERWISE IT WILL JUST BE SKIPPED
    {
        isBuiltIn = true;
        if(strcmp(cmd, SH_DIE) == 0 && commands[0].arguments->data[1] != NULL)
        //check for die args
        {
            HandleDie(commands);
            LAST_EXIT_STATUS = 1;
        }
        else
        {
            printf("mysh: Exiting mysh.....\n"); 
        }
        EXIT_ = true;
        LAST_EXIT_STATUS = (strcmp(cmd, SH_DIE) == 0) ? 1 : 0;
    }

    CleanUp(commands, numCommands);
}



void CleanUp(CommandData commandData[], int numCommands)
{
    for(int i = 0; i<numCommands; i++)
    {
        Free_Command(&commandData[i]);
    }
}
