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
mutexalloc(struct file **f0) {
    printf("using mutexalloc\n");
    struct sleeplock *lk;

    lk = 0;
    *f0 = 0;
    if ((*f0 = filealloc()) == 0)
        goto bad;
    if ((lk = (struct sleeplock *) kalloc()) == 0)
        goto bad;
    printf("using kalloc\n");
    initsleeplock(lk, "mutex");
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
    printf("using mutexclose\n");
    kfree((char *) lk);
    printf("using kfree\n");
}
