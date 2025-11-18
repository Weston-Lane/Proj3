#include <stdio.h>

int main(void) {
    fprintf(stdout, "STDOUT: hello from test_out_err\n");
    fprintf(stderr, "STDERR: this should stay on the terminal\n");
    return 0;
}
