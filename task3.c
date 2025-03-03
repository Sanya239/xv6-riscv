
// USE
// gcc task3.c -o task3
// ./task3 [args]
// TO COMPILE AND RUN THIS FILE

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include <unistd.h>
#include <sys/wait.h>


int
main(int argc, char *argv[]) {
    int pipe_ends[2];

    if (pipe(pipe_ends) < 0) {
        fprintf(stderr, "Failed to open a pipe\n");
        return 1;
    }

    int child_id;
    child_id = fork();
    if (child_id < 0) {
        fprintf(stderr, "Failed to fork process\n");
        exit(2);
    }
    if (child_id > 0) {
        close(pipe_ends[0]);
        for (int i = 0; i < argc; i++) {
            int to_write = strlen(argv[i]);
            int written = 0;
            while (to_write > 0) {
                written += write(pipe_ends[1], argv[i] + written, to_write);
                if (written < 0) {
                    fprintf(stderr, "Failed to write into the pipe\n");
                }
                to_write -= written;
            }
            if (write(pipe_ends[1], " ", 1) != 1) {
                fprintf(stderr, "Failed to write into the pipe\n");
            }
        }
        write(pipe_ends[1], "\n", 1);

        close(pipe_ends[1]);
        int status;
        wait(&status);
        printf("Child process exit code: %d\n", status);
    }
    if (child_id == 0) {
        close(pipe_ends[1]);
        close(0);
        char buf[100];
        int r = 0;
        while ((r = read(pipe_ends[0], buf, 99)) != 0) {
            buf[r] = 0;
            int written = 0;
            while (written < r + 1) {
                written += write(1, buf + written, r + 1 - written);
            }
        }
    }
}

