#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "spinlock.h"
#include "proc.h"
#include "fs.h"
#include "sleeplock.h"
#include "file.h"

#define PIPESIZE 512

int
mutexalloc(struct file **f0, char* name) {
    printf("using mutexalloc on %s\n", name);
    struct sleeplock *lk;
    lk = 0;
    *f0 = 0;
    if ((*f0 = filealloc()) == 0)
        goto bad;
    if ((lk = (struct sleeplock *) kalloc()) == 0)
        goto bad;
    printf("using kalloc in %lx\n", (uint64)lk);
    initsleeplock(lk, name);
    (*f0)->type = FD_MUTEX;

    (*f0)->readable = 0;
    (*f0)->writable = 0;
    (*f0)->mutex = lk;
    return 0;

    bad:
    if (lk)
        kfree((char *) lk);
    if (*f0)
        fileclose(*f0);
    return -1;
}

void
mutexclose(struct sleeplock *lk) {
    printf("using mutexclose on %s\n", lk->name);
    printf("using kfree in %lx\n", (uint64) lk);
    kfree(lk->name);
    kfree((char *) lk);
}
