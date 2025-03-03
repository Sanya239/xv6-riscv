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
    int cild_id = fork();
    if (cild_id < 0) {
        fprintf(2, "Failed to fork process\n");
        exit(1);
    }
    if (cild_id > 0) {
        printf("parent process id: %d\nchild process id: %d\n", getpid(), cild_id);
        int status;
        if (use_kill) {
            status = kill(cild_id);
        } else {
            wait(&status);
        }
        printf("child process exit code: %d\n", status);
    }
    if (cild_id == 0) {
        sleep(time_to_sleep);
        exit(1);
    }

}
