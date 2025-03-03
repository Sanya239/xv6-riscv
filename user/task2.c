#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[]) {
    int pipe_ends[2];

    if (pipe(pipe_ends) < 0) {
        fprintf(2, "Failed to open a pipe\n");
        return 1;
    }

    int child_id;
    child_id = fork();
    if (child_id < 0) {
        fprintf(2, "Failed to fork process\n");
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
                    fprintf(2, "Failed to write into the pipe\n");
                }
                to_write -= written;
            }
            if (write(pipe_ends[1], " ", 1) != 1) {
                fprintf(2, "Failed to write into the pipe\n");
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
        dup(pipe_ends[0]);
        close(pipe_ends[0]);
        char *argv_[] = {"/wc", 0};
        exec("/wc", argv_);
    }
}