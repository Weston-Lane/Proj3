#include "PipeLineExecution.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>    // For 'bool' type
#include <unistd.h>     // For fork, execv, pipe, dup2, STDIN_FILENO, STDOUT_FILENO, chdir, getcwd, access
#include <sys/wait.h>   // For waitpid
#include <sys/types.h>  // For pid_t
#include <fcntl.h>      // For open, O_RDONLY, O_WRONLY, O_CREAT, O_TRUNC
#include <sys/stat.h>

#include "MacDef.h"
char* FindFullPath(char* command);
void FreeFullPath(char* path_to_free, char* command);
int HandleWhich(char* args[]);

int ExecutePipeLine(CommandData commands[], int numCommands) {
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

    for (int i = 0; i < numCommands; i++) 
    {
        int new_pipe[2];
        
        if (i < numCommands - 1) {
            if (pipe(new_pipe) == -1) 
            {
                perror("mysh: pipe");
                return 1;
            }
        }

        pids[i] = fork();

        if (pids[i] < 0) 
        {
            perror("mysh: fork");
            return 1;
        }
        /************IN CHILD********************************/
        if (pids[i] == 0) 
        {
            
            if (in_fd != STDIN_FILENO) 
            {
                dup2(in_fd, STDIN_FILENO);
                close(in_fd); 
            }
            if (i < numCommands - 1) 
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
            
            if (strcmp(cmd, "pwd") == 0) {
                
             
                char cwd[1024];
                if (getcwd(cwd, sizeof(cwd)) != NULL) {
                    printf("%s\n", cwd);
                    exit(0); 
                } else {
                    perror("mysh: pwd");
                    exit(1); 
                }
            }
            else if (strcmp(cmd, "which") == 0) {
             
                int which_status = HandleWhich(commands[i].arguments->data);
                exit(which_status);
            }
 
            else if (strcmp(cmd, "exit") == 0) {
     
                exit(0);
            }
            else if (strcmp(cmd, "die") == 0) {
   
                for (int j = 1; commands[i].arguments->data[j] != NULL; j++) {
                    fprintf(stderr, "%s ", commands[i].arguments->data[j]);
                }
                fprintf(stderr, "\n");
            
                exit(1);
            }

            PRINT("COMMAND: %s\n", commands[i].arguments->data[0]);
            char* full_path = FindFullPath(commands[i].arguments->data[0]);
            PRINT("FULL PATH: %s\n", full_path);
            if (full_path == NULL) {
                fprintf(stderr, "mysh: command not found: %s\n", commands[i].arguments->data[0]);
                exit(127);
            }
            
            execv(full_path, commands[i].arguments->data);
            
            // execv ONLY returns on error
            perror("mysh: execv");
            FreeFullPath(full_path, commands[i].arguments->data[0]);
            exit(1);
        }

        /************IN PARENT********************************/
        if (in_fd != STDIN_FILENO) {
            close(in_fd);
        }
        if (i < numCommands - 1) {
            close(new_pipe[1]);
        }
        if (i < numCommands - 1) {
            in_fd = new_pipe[0];
        }
    }
    
    // --- 9. PARENT WAITS for all children ---
    for (int i = 0; i < numCommands; i++) {
        int status;
        waitpid(pids[i], &status, 0);
        
        // Save the exit status of the *last* command
        if (i == numCommands - 1) {
            if (WIFEXITED(status)) {
                finalStatus = WEXITSTATUS(status);
            } else {
                finalStatus = 1;
            }
        }
    }

    return finalStatus;
}


/*
 * =======================================================
 * Helper Functions (You must implement these)
 * =======================================================
 */

// This is your function that searches mandated paths
// (per the new spec)
char* FindFullPath(char* command) {
    // 1. Check for absolute or relative path (contains '/')
    if (strchr(command, '/') != NULL) {
        if (access(command, X_OK) == 0) {
            return command; // It's a valid path, return it as-is
        } else {
            return NULL; // Path given, but it doesn't exist
        }
    }

    // 2. Search mandated paths (per the new spec)
    const char* paths[] = {"/usr/local/bin", "/usr/bin", "/bin", NULL};
    
    for (int j = 0; paths[j] != NULL; j++) {
        char test_path[BUFFER_SIZE];
        snprintf(test_path, sizeof(test_path), "%s/%s", paths[j], command);
        PRINT("FULL PATH: %s\n", test_path);
        if (access(test_path, X_OK) == 0) {
            // FOUND IT!
            PRINT("FOUND\n");
            return strdup(test_path); // Return a *new* copy
        }
    }

    // 3. Not found
    return NULL;
}

// Helper to free the path *only* if we strdup'd it
void FreeFullPath(char* path_to_free, char* command) {
    // If the path is the same as the command (e.g. "/bin/ls")
    // then we did *not* strdup it.
    if (path_to_free != command) {
        free(path_to_free);
    }
}

// --- UPDATED 'which' HELPER FUNCTION (per your new spec) ---
int HandleWhich(char* args[]) {
    if (args[1] == NULL) {
        // "fails if it is given the wrong number of arguments"
        // (prints nothing, as per the spec)
        return 1; // Failure
    }

    char* cmd_to_find = args[1];
    
    // 1. Check if it's a built-in
    if (strcmp(cmd_to_find, "cd") == 0 ||
        strcmp(cmd_to_find, "pwd") == 0 ||
        strcmp(cmd_to_find, "which") == 0 ||
        strcmp(cmd_to_find, "exit") == 0 ||
        strcmp(cmd_to_find, "die") == 0) 
    {
        // "prints nothing and fails if... the name of a built-in"
        return 1; // Failure
    }

    // 2. If not, search for it using the *exact* same path logic
    char* full_path = FindFullPath(cmd_to_find);

    if (full_path != NULL) {
        printf("%s\n", full_path);
        // Free the path *only* if FindFullPath strdup'd it
        FreeFullPath(full_path, cmd_to_find);
        return 0; // Success
    }
    
    // "prints nothing and fails if... the program is not found"
    return 1; // Not found, failure
}