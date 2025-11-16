#include <stdio.h>

/**
 * This program always succeeds.
 * It prints to stderr so its output doesn't
 * interfere with stdout redirection tests.
 */
int main() {
    fprintf(stderr, "[test_succeed: running and exiting with 0]\n");
    return 0; // 0 = success
}