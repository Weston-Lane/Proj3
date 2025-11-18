#include <stdio.h>
#include <stdlib.h>

/*
 * Usage: test_status N
 * Prints N and exits with code N.
 */
int main(int argc, char *argv[]) {
    int code = 0;
    if (argc > 1) {
        code = atoi(argv[1]);
    }
    fprintf(stderr, "[test_status: exiting with %d]\n", code);
    return code;
}
