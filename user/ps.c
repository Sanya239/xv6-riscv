#include "kernel/types.h"
#include "kernel/spinlock.h"
#include "user/user.h"


static char *states[] = {
        "unused",
        "used",
        "sleep ",
        "runble",
        "run   ",
        "zombie"
};

int
main(int argc, char *argv[]) {
    int proc_count = ps_listinfo(0, 0);
    struct procinfo *list = malloc(proc_count * sizeof(struct procinfo));

    int res = ps_listinfo((char *) list, proc_count);

    if (res == -1) {
        free(list);
        fprintf(2, "ps_listinfo failed to write into list\n");
        exit(1);
    }

    while (res > proc_count) {
        proc_count += 10;
        free(list);
        list = malloc(proc_count * sizeof(struct procinfo));

        res = ps_listinfo((char *) list, proc_count);
        if (res == -1) {
            free(list);
            fprintf(2, "ps_listinfo failed to write into list\n");
            exit(1);
        }
    }
    fprintf(1, "pid\tname\tstate\tppid\tppname\n");
    for (int i = 0; i < res; i++) {
        fprintf(1, "%d\t%s\t%s\t%d\t%s\n", list[i].pid, list[i].name, states[list[i].state], list[i].parent_id,
                list[i].parent_name);
    }
    free(list);
}