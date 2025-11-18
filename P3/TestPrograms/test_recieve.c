#include <stdio.h>
#include <string.h>

/**
 * This program reads one line from STANDARD INPUT
 * and prints a message to STANDARD OUTPUT.
 * Used to test file redirection (<) and piping (|).
 */
int main() {
    char buffer[1024];
    
    // Read one line from stdin
    if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        // Print what we received to stdout
        printf("Receiver got: %s", buffer);
        fflush(stdout);
    } else {
        // This will happen if input is closed (e.g., Ctrl+D)
        fprintf(stderr, "[test_receiver: received no input]\n");
        return 1;
    }
    
    return 0;
}