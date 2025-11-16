#include "PipeLineExecution.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>   
#include <sys/wait.h> 
#include <sys/types.h>
#include <fcntl.h>    
#include <sys/stat.h>

#include "MacDef.h"
#include "BuiltIns.h"

int ExecutePipeLine(CommandData commands[], int numCommands) 
{
    #if DEBUG
        printf("--- Printing all commands after loop ---\n");
        for (int j = 0; j < numCommands; j++) 
            { Print_Command(&commands[j]); }
    #endif
    
    if (numCommands < 1)
    //only one command
        { return 0; }

    pid_t pids[numCommands];
    int in_fd = STDIN_FILENO;
    int finalStatus = 0;

    //loop through commands
    for (int i = 0; i < numCommands; i++) 
    {
        //create pipes
        int new_pipe[2];
        
        if (i < numCommands - 1) {
            if (pipe(new_pipe) == -1) 
            {
                perror("mysh: pipe");
                return 1;
            }
        }

        pids[i] = fork();
        //create child
        if (pids[i] < 0) 
        {
            perror("mysh: fork");
            return 1;
        }
        /************IN CHILD********************************/
        if (pids[i] == 0) 
        {
            
            if (in_fd != STDIN_FILENO)
            //if STDIN has been changed from other child
            {
                dup2(in_fd, STDIN_FILENO);
                close(in_fd); 
            }
            if (i < numCommands - 1) 
            //if not last command or only 1 command
            {
                dup2(new_pipe[1], STDOUT_FILENO);
                close(new_pipe[0]);
                close(new_pipe[1]);
            }
            
            if (commands[i].inFile != NULL) 
            {
                int in_file_fd = open(commands[i].inFile, O_RDONLY);
                if (in_file_fd < 0) {
                  
                    perror("mysh"); 
                    exit(1);
                }
                dup2(in_file_fd, STDIN_FILENO);
                close(in_file_fd);
            }
            
            if (commands[i].outFile != NULL) 
            {
                int out_file_fd = open(commands[i].outFile, 
                                       O_WRONLY | O_CREAT | O_TRUNC, 
                                       S_IRUSR | S_IWUSR | S_IRGRP);
                if (out_file_fd < 0) 
                {
                    perror("mysh");
                    exit(1);
                }
                dup2(out_file_fd, STDOUT_FILENO);
                close(out_file_fd);
            }

            if (commands[i].arguments->data[0] == NULL) 
                { exit(0); }
            
            char* cmd = commands[i].arguments->data[0];
            if (strcmp(cmd, SH_PWD) == 0)
            //check pwd
            { 
                int pwdStatus = HandlePWD();
                exit(pwdStatus);
            }
            else if (strcmp(cmd, SH_WHICH) == 0) 
            {
                int which_status = HandleWhich(commands[i].arguments->data);
                exit(which_status);
            }
 
            else if (strcmp(cmd, SH_EXIT) == 0) 
            {
                printf("mysh: Exiting mysh.....\n"); 
                exit(0); 
            }

            else if (strcmp(cmd, SH_DIE) == 0) 
            {
                // for (int j = 1; commands[i].arguments->data[j] != NULL; j++) 
                //     { fprintf(stderr, "%s ", commands[i].arguments->data[j]); }

                // fprintf(stderr, "\n");
                exit(1);
            }

            PRINT("COMMAND: %s\n", commands[i].arguments->data[0]);
            char* full_path = FindFullPath(commands[i].arguments->data[0]);
            PRINT("FULL PATH: %s\n", full_path);
            if (full_path == NULL) 
            {
                fprintf(stderr, "mysh: command not found: %s\n", commands[i].arguments->data[0]);
                exit(127);
            }

            //exec to new Program
            execv(full_path, commands[i].arguments->data);
            
            //execv ONLY returns on error
            perror("mysh: execv");
            FreeFullPath(full_path, commands[i].arguments->data[0]);
            exit(1);
        }

        /************IN PARENT********************************/
        if (in_fd != STDIN_FILENO) 
            { close(in_fd); }
        if (i < numCommands - 1)
            { close(new_pipe[1]); }
        if (i < numCommands - 1)
            { in_fd = new_pipe[0]; }
    }
    
    /*********************Wait for children*****************************/
    for (int i = 0; i < numCommands; i++) 
    {
        int status;
        waitpid(pids[i], &status, 0);
        
        // Save the exit status of the *last* command
        if (i == numCommands - 1) 
        {
            if (WIFEXITED(status)) 
                { finalStatus = WEXITSTATUS(status); } 
            else 
                { finalStatus = 1; }
        }
    }

    return finalStatus;
}




