#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "spinlock.h"
#include "proc.h"
#include "defs.h"

#define CLK_LOW (*((volatile  uint32 *)GOLDFISHRTC_LOW))
#define CLK_HIGH (*(((volatile uint32 *)GOLDFISHRTC_LOW)+1))

uint64 rtc_read(void) {
  return (uint64) CLK_LOW + ((uint64) CLK_HIGH << 32);
}

int sys_rtc(void) {
  uint64 t;
  argaddr(0, &t);
  uint64 time = rtc_read();
  printf("%lx\n", time);
  copyout(myproc()->pagetable, t, (char *) &time, sizeof(time));
  return 0;
}
