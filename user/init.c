// init: The initial user-level program

#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/spinlock.h"
#include "kernel/sleeplock.h"
#include "kernel/fs.h"
#include "kernel/file.h"
#include "user/user.h"
#include "kernel/fcntl.h"

char *argv[] = { "sh", 0 };

int
main(void)
{
  int pid, wpid;
  printf("init: starting\n");
  if(open("console", O_RDWR) < 0){
    mknod("console", CONSOLE, 0);
    open("console", O_RDWR);
  }
  printf("init: starting xv6\n");
  if(open("mydevice0", O_RDWR) < 0){
    mknod("mydevice0", MYDEVICE, 0);
    open("mydevice0", O_RDWR);
  }
  printf("init: starting xv7\n");
  if(open("mydevice1", O_RDWR) < 0){
    mknod("mydevice1", MYDEVICE, 1);
    open("mydevice1", O_RDWR);
  }
  printf("init: starting xv8\n");
  if(open("mydevice2", O_RDWR) < 0){
    mknod("mydevice2", MYDEVICE, 2);
    open("mydevice2", O_RDWR);
  }
  if(open("mydevice3", O_RDWR) < 0){
    mknod("mydevice3", MYDEVICE, 3);
    open("mydevice3", O_RDWR);
  }
  printf("init: starting xv9\n");
  dup(0);  // stdout
  dup(0);  // stderr

  for(;;){
    printf("init: starting sh\n");
    pid = fork();
    if(pid < 0){
      printf("init: fork failed\n");
      exit(1);
    }
    if(pid == 0){
      exec("sh", argv);
      printf("init: exec sh failed\n");
      exit(1);
    }

    for(;;){
      // this call to wait() returns if the shell exits,
      // or if a parentless process exits.
      wpid = wait((int *) 0);
      if(wpid == pid){
        // the shell exited; restart it.
        break;
      } else if(wpid < 0){
        printf("init: wait returned an error\n");
        exit(1);
      } else {
        // it was a parentless process; do nothing.
      }
    }
  }
}
