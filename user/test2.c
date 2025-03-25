#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int read_write() {
    int mx, err = mutex(&mx);
    if (err != 0) {
        fprintf(2, "mutex() failed \n");
        return err;
    }
    err = write(mx, "b", 1);
    if (err == 0) {
        fprintf(2, "read_write_failed\n");
        return 1;
    }
    err = read(mx, "b", 1);
    if (err == 0) {
        fprintf(2, "read_write_failed\n");
        return 2;
    }
    return 0;
}

int close1() {
    int mx, err = mutex(&mx);
    if (err != 0) {
        fprintf(2, "mutex() failed \n");
        return -1;
    }
    err = mutex_lock(mx);
    if (err != 0) {
        fprintf(2, "mutex_lock() failed \n");
        return -1;
    }
    err = close(mx);
    if (err != 0) {
        fprintf(2, "close() failed\n");
        return -1;
    }
    err = mutex_unlock(mx);
    if (err == 0) {
        fprintf(2, "close1 failed\n");
        return -1;
    }

    return 0;
}

int close2() {
    int mx, err = mutex(&mx);
    if (err != 0) {
        fprintf(2, "mutex_lock failed\n");
        return -1;
    }
    int id = fork();
    if (id == 0) {
        err = mutex_lock(mx);
        if (err != 0) {
            fprintf(2, "mutex_lock() failed \n");
            return -1;
        }
        wait(0);
        err= mutex_unlock(mx);
        if (err != 0) {
            fprintf(2, "close2 failed\n");
            return -1;
        }
    } else {
        sleep(50);
        err = close(mx);
        if (err != 0) {
            fprintf(2, "close() failed\n");
            return -1;
        }
        exit(0);
    }
    return 0;
}

int free1() {
    int mx, err = mutex(&mx);
    if (err != 0) {
        fprintf(2, "mutex_lock failed\n");
        return -1;
    }
    int id = fork();
    if (id == 0) {
        wait(0);
        if (mutex_unlock(mx) == 0) {
            fprintf(2, "free failed\n");
            return 1;
        }
    } else {
        mutex_lock(mx);
        exit(0);
    }
    return 0;
}

int main(int argc, char *argv[]) {
    if (read_write() == 0) {
        fprintf(0, "\nread_write ok\n");
    }
    if (close1() == 0) {
        fprintf(0, "\nclose1 ok\n");
    }
    if (close2() == 0) {

        fprintf(0, "\nok\n");
    }
    if (free1() == 0) {
        fprintf(0, "\nfree ok\n");
    }
    exit(0);
}
