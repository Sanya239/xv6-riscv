
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
        perror("Failed to open a pipe\n");
        exit(1);
    }

    int child_id;
    child_id = fork();
    if (child_id < 0) {
        perror("Failed to fork process\n");
        exit(2);
    }
    if (child_id > 0) {

        int success = close(pipe_ends[0]);
        if (success == -1) {
            perror("Parent to close the pipe\n");
            exit(3);
        }
        for (int i = 0; i < argc; i++) {
            int to_write = strlen(argv[i]);
            ssize_t written = 0;
            while (to_write > 0) {
                written += write(pipe_ends[1], argv[i] + written, to_write);
                if (written < 0) {
                    perror("Failed to write into the pipe\n");
                    exit(2);
                }
                to_write -= written;
            }
            if (write(pipe_ends[1], "\n", 1) != (ssize_t) 1) {
                perror("Failed to write into the pipe\n");
                exit(2);
            }
        }

        success = close(pipe_ends[1]);
        if (success == -1) {
            perror("Parent to close the pipe\n");
            exit(3);
        }
        int status;
        wait(&status);
        if (status == -1) {
            perror("wait failed");
            exit(2);
        }
        printf("Child process exit code: %d\n", status);
    }
    if (child_id == 0) {
        int success = close(pipe_ends[1]);
        if (success == -1) {
            perror("Child to close the pipe\n");
            exit(3);
        }
        success = close(0);
        if (success == -1) {
            perror("Child to close stdin\n");
            exit(3);
        }
        char buf[100];
        int r = 0;
        while ((r = read(pipe_ends[0], buf, 99)) != 0) {
            buf[r] = 0;
            ssize_t written = 0;
            while (written < r + 1) {
                written += write(1, buf + written, r + 1 - written);
            }
        }
    }
}

