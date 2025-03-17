#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"
#include "kernel/spinlock.h"
#include "kernel/riscv.h"
#include "kernel/proc.h"


//static char *states[] = {
//        [UNUSED]    "unused",
//        [USED]      "used",
//        [SLEEPING]  "sleep ",
//        [RUNNABLE]  "runble",
//        [RUNNING]   "run   ",
//        [ZOMBIE]    "zombie"
//};

void test_not_enough_memory() {
    fprintf(1, "test_not_enough_memory: ");
    char *list = malloc(2 * sizeof(struct procinfo));
    int res = ps_listinfo(list, 2);
    free(list);
    fprintf(1, "%s\n", (res > 2) ? "ok" : "failed");
}

void test_limit_exceeded() {
    fprintf(1, "test_limit_exceeded: ");
    int res = ps_listinfo((char *) 1, 100);
    fprintf(1, "%s\n", (res == -1) ? "ok" : "failed");
}

void test_count_only() {
    fprintf(1, "test_count_only: ");
    int res = ps_listinfo(0, 0);
    fprintf(1, "%s\n", (res > 0) ? "ok" : "failed");
}

void test_normal() {
    fprintf(1, "test_normal: ");
    int proc_count = ps_listinfo(0, 0);
    struct procinfo *list = malloc(proc_count * sizeof(struct procinfo));

    int res = ps_listinfo((char *) list, proc_count);
    if (res == -1) {
        fprintf(1, "%s\n", "failed");
    }

    while (res > proc_count) {
        proc_count += 10;
        free(list);
        list = malloc(proc_count * sizeof(struct procinfo));

        res = ps_listinfo((char *) list, proc_count);
        if (res == -1) {
            fprintf(1, "%s\n", "failed");
        }
    }
    free(list);
    fprintf(1, "%s\n", "ok");
}

int
main(int argc, char *argv[]) {
    test_not_enough_memory();
    test_limit_exceeded();
    test_count_only();
    test_normal();
}