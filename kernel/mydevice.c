#include <stdarg.h>

#include "types.h"
#include "param.h"
#include "spinlock.h"
#include "sleeplock.h"
#include "fs.h"
#include "file.h"
#include "memlayout.h"
#include "riscv.h"
#include "defs.h"
#include "proc.h"

#define BACKSPACE 0x100
#define C(x)  ((x)-'@')  // Control-x


struct {
    struct spinlock lock;

    // input
#define INPUT_BUF_SIZE 128
    char buf[INPUT_BUF_SIZE];

#define MYDEVICE_MULTIPLIER 6364136223846793005ULL
#define MYDEVICE_INCREMENT 1442695040888963407ULL
    uint64 urandom_seed;
    uint64 nullstat_count;
} mydevice;


int
mydevicewrite(short minor, int user_src, uint64 src, int n) {
  if (minor == 0) {
    // divice null
    return n;
  }
  if (minor == 1) {
    return -1;
  }
  if (minor == 2) {
    if (n > sizeof(mydevice.urandom_seed)) {
      return -1;
    }
    acquire(&mydevice.lock);
    if (either_copyin(&mydevice.urandom_seed, user_src, src, sizeof(mydevice.urandom_seed)) == -1) {
      release(&mydevice.lock);
      return -1;
    }
    release(&mydevice.lock);
    return sizeof(mydevice.urandom_seed);
  }
  if (minor == 3) {
    acquire(&mydevice.lock);
    mydevice.nullstat_count += n;
    release(&mydevice.lock);
    return n;
  }
  return -1;  // default case for invalid minor devices
}


int
mydeviceread(short minor, int user_dst, uint64 dst, int n) {

  if (minor == 0) {
    return 0;
  }
  if (minor == 1) {
    uint target;
    char cbuf;
    target = n;
    while (n > 0) {
      cbuf = 0;
      if (either_copyout(user_dst, dst, &cbuf, 1) == -1)
        break;

      dst++;
      --n;
    }
    return target - n;
  }
  if (minor == 2) {
    acquire(&mydevice.lock);
    mydevice.urandom_seed = mydevice.urandom_seed * MYDEVICE_MULTIPLIER + MYDEVICE_INCREMENT;
    uint64 random_value = mydevice.urandom_seed;
    release(&mydevice.lock);

    if (n != sizeof(uint64)) {
      return -1;
    }
    if (either_copyout(user_dst, dst, (char *) &random_value, sizeof (uint64)) == -1)
      return -1;
    return sizeof(uint64);
  }
  if (minor == 3) {
    return mydevice.nullstat_count;
  }

  release(&mydevice.lock);
  return -1;
}

void
mydeviceinit(void) {
  initlock(&mydevice.lock, "mydevice");

  // Initialize the random seed with a default value
  mydevice.urandom_seed = 0x12345678;
  mydevice.nullstat_count = 0;

  // Connect read and write system calls
  devsw[MYDEVICE].read = mydeviceread;
  devsw[MYDEVICE].write = mydevicewrite;
}
