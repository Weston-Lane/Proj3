#include <stdio.h>

/*
 * Reads one line from stdin (if any) and prints what it saw.
 */
int main(void) {
    char buf[256];

    if (fgets(buf, sizeof(buf), stdin) == NULL) {
        printf("[test_stdin: no input]\n");
    } else {
        printf("[test_stdin: got: %s", buf);
        if (buf[0] && buf[0] != '\n') {
            printf("]\n");
        }
    }

    return 0;
}
