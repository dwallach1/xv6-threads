#include "types.h"
#include "x86.h"
#include "defs.h"
#include "param.h"
#include "mmu.h"
#include "proc.h"
#include "sysfunc.h"

struct lock_t{
  uint locked; //is the lock held? 0 for no, 1 for yes
};

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return proc->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = proc->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;
  
  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(proc->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since boot.
int
sys_uptime(void)
{
  uint xticks;
  
  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int
sys_clone(void)
{
  void* fcn;
  void* arg;
  void* stack;
  int size = sizeof(void *);

  if(argptr(0, (char**) &fcn, size) < 0)
    return -1;
  if(argptr(1, (char**) &arg, size) < 0)
    return -1;
  if(argptr(2, (char**) &stack, size) < 0)
    return -1;

  if((uint)stack % 4096 != 0)
    return -1;

  if((uint)stack + 4096 > proc->sz)
    return -1;

  return clone(fcn, arg, stack);
}

int
sys_join(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;

  return join((int)pid);
}

int
sys_threadsleep(void)
{
  void* chan;
  lock_t* lock;
  int size = sizeof(void *);

  if(argptr(0, (char**) &chan, size) < 0)
    return -1;
  if(argptr(1, (char **) &lock, size) < 0)
    return -1;
  threadsleep(chan, lock);
  return 0;
}

int 
sys_threadwakeup(void)
{
  void* chan;
  int size = sizeof(void *);

  if(argptr(0, (char**) &chan, size) < 0)
    return -1;

  threadwakeup(chan);
  return 0;
}
