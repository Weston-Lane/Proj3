#ifndef COMMD_H
#define COMMD_H

#include "ArrayList.h"

typedef struct CommandData
{
    ArrayList* arguments;
    char* inFile;
    char* outFile;
}CommandData;

void CommandData_CreateCommand(CommandData* commandData, char* line, int length);
void Print_Command(CommandData* commandData);
void Free_Command(CommandData* commandData);

#endif