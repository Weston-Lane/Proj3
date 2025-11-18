#include <stdio.h>

/*
 * Reads from stdin until EOF and writes each line to stdout
 * prefixed with a line number.
 */
int main(void) {
    char buf[256];
    int line = 1;

    while (fgets(buf, sizeof buf, stdin) != NULL) {
        printf("[%d] %s", line, buf);
        line++;
    }

    return 0;
}
