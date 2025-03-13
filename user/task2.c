#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[]) {
    int pipe_ends[2];

    if (pipe(pipe_ends) < 0) {
        fprintf(2, "Failed to open a pipe\n");
        exit(1);
    }

    int child_id;
    child_id = fork();
    if (child_id < 0) {
        fprintf(2, "Failed to fork process\n");
        exit(2);
    }
    if (child_id > 0) {
        int success = close(pipe_ends[0]);
        if (success == -1) {
            fprintf(2, "Parent to close the pipe\n");
            exit(3);
        }
        for (int i = 0; i < argc; i++) {
            int to_write = strlen(argv[i]);
            int written = 0;
            while (to_write > 0) {
                written += write(pipe_ends[1], argv[i] + written, to_write);
                if (written < 0) {
                    fprintf(2, "Failed to write into the pipe\n");
                    exit(100);
                }
                to_write -= written;
            }
            if (write(pipe_ends[1], "\n", 1) != 1) {
                fprintf(2, "Failed to write into the pipe\n");
                exit(100);
            }
        }

        success = close(pipe_ends[1]);
        if (success == -1) {
            fprintf(2, "Parent to close the pipe\n");
            exit(3);
        }
        int status;
        wait(&status);
        if (status == -1) {
            fprintf(2, "wait failed");
            exit(2);
        }
        printf("Child process exit code: %d\n", status);
    }
    if (child_id == 0) {
        int success = close(pipe_ends[1]);
        if (success == -1) {
            fprintf(2, "Child to close the pipe\n");
            exit(3);
        }
        success = close(0);
        if (success == -1) {
            fprintf(2, "Child to close the pipe\n");
            exit(3);
        }
        dup(pipe_ends[0]);
        success = close(pipe_ends[0]);
        if (success == -1) {
            fprintf(2, "Child to close the pipe\n");
            exit(3);
        }
        char *argv_[] = {"/wc", 0};
        exec("/wc", argv_);
    }
}