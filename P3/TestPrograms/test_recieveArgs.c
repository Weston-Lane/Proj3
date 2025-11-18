#include <stdio.h>
#include <string.h>

/**
 * This program reads one line from STANDARD INPUT
 * and prints a message to STANDARD OUTPUT.
 * Used to test file redirection (<) and piping (|).
 */
int main(int argc, char** argv) 
{
    if(argv[0] != NULL)
    {
        printf("ARGS: ");
        for(int i = 0; i < argc; i++)
        {
            printf("%s, ",argv[i]);
        }
        printf("\n");
        
        fflush(stdout);
    }
    
    
    return 0;
}