#include <stdio.h>

/**
 * This program always fails.
 * It prints to stderr so its output doesn't
 * interfere with stdout redirection tests.
 */
int main() {
    fprintf(stderr, "[test_fail: running and exiting with 1]\n");
    return 1; // 1 (or any non-zero) = failure
}