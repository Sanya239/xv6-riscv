#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Not enough args\n");
        exit(1);
    }

    int mx;
    int err = mutex(&mx);
    if (err != 0) {
        fprintf(2, "mutex() err\n");
    }
    int pid = fork();
    if (pid == 0) {
        int pid = getpid();
        for (int i = 1; i < argc; i++) {
            for (int j = 0; argv[i][j] != '\0'; j++) {
                mutex_lock(mx);
                printf("pid: %d, char %c\n", pid, argv[i][j]);
                mutex_unlock(mx);
            }
        }
        exit(0);
    }

    pid = fork();
    if (pid == 0) {
        int pid = getpid();
        for (int i = 1; i < argc; i++) {
            for (int j = 0; argv[i][j] != '\0'; j++) {
                mutex_lock(mx);
                printf("pid: %d, char %c\n", pid, argv[i][j]);
                mutex_unlock(mx);
            }
        }
        exit(0);
    }
    wait(0);
    wait(0);

    exit(0);
}
