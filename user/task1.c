// RUN WITHOUT FLAGS TO SIMULATE TASK 1a
// RUN WITH FLAG --kill TO SIMULATE TASK 2a

#include "kernel/types.h"
#include "user/user.h"

int use_kill = 0;
int time_to_sleep = 50; // time in tenth of a second
int
main(int argc, char *argv[]) {
    if (argc == 2) {
        if (strcmp(argv[1], "--kill") == 0) {
            use_kill = 1;
        }
    }
    int child_id = fork();
    if (child_id < 0) {
        fprintf(2, "Failed to fork process\n");
        exit(1);
    }
    if (child_id > 0) {
        printf("Parent process id: %d\nchild process id: %d\n", getpid(), child_id);
        int status;
        if (use_kill) {
            status = kill(child_id);
            if (status == -1) {
                fprintf(2, "Failed to kill child process");
                exit(2);
            }
        } else {
            wait(&status);
            if (status == -1) {
                fprintf(2, "wait failed");
                exit(2);
            }
        }
        printf("Child process exit code: %d\n", status);
        exit(0);
    }
    if (child_id == 0) {
        sleep(time_to_sleep);
        exit(0);
    }

}
