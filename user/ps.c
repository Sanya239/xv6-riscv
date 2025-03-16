#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"
#include "kernel/spinlock.h"
#include "kernel/riscv.h"
#include "kernel/proc.h"

//
//static char *states[] = {
//        [UNUSED]    "unused",
//        [USED]      "used",
//        [SLEEPING]  "sleep ",
//        [RUNNABLE]  "runble",
//        [RUNNING]   "run   ",
//        [ZOMBIE]    "zombie"
//};

int
main(int argc, char *argv[]) {
    struct procinfo *list = malloc(100 * sizeof(struct procinfo));
    int res = ps_listinfo((char *) list, 100);
    fprintf(1, "ok\n%d\nok\n", res);
    for (int i = 0; i < res; i++) {
        fprintf(1, "%d\t%s\n", list[i].pid, list[i].name);
    }
    fprintf(1, "\nbebebebe\n");

    free(list);
}