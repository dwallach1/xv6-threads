#include "types.h"
#include "stat.h"
#include "user.h"
#include "x86.h"
#define PGSIZE (4096)
#define numUserThreads (64)
// typedef struct lock_t{
// 	uint locked; //is the lock held? 0 for no, 1 for yes
// };

struct thread {
	void* stackAddr;
	int pid;
	int used;
};

struct thread threads[numUserThreads];


int
thread_create(void (*start_routine)(void*), void* arg)
{
  void* stack = malloc(2*PGSIZE);
  if((uint)stack % PGSIZE){
     stack = stack + (PGSIZE - (uint)stack % PGSIZE);
  } 
  int pid = clone(start_routine, arg, stack);

  // find next free space in userthreads array
  int i;
  int success = 0;
  for(i = 0; i < numUserThreads; i++){
  	if(threads[i].used == 0){
  		// printf(1, "used is %d\n", threads[i].used);
  		threads[i].stackAddr = stack;
  		threads[i].pid = pid;
  		threads[i].used = 1;
  		success = 1;
  		break;
  	}
  }

  if(success != 1){
  	printf(1, "returning no success %d\n", success);
  	return -1;
  }

  return pid;
}

int 
thread_join(pid)
{
  int result = join(pid);
  if(result == -1)
    return -1;

 int i;
 for(i=0; i < numUserThreads; i++){
 	if(threads[i].pid == result){
 		free(threads[i].stackAddr);
 		threads[i].used = 0;
 		threads[i].pid = 0;
 		break;
 	}
 }
  return result;
}

void
lock_acquire(lock_t* lock)
{
  while(xchg(&lock->locked, 1) != 0)
  	;
  
}

void
lock_release(lock_t* lock)
{

	xchg(&lock->locked, 0);
}

void
lock_init(lock_t* lock)
{
  lock->locked = 0;
}

void
cv_wait(cond_t* conditionVariable, lock_t* lock)
{
	xchg(&lock->locked, 1);
	threadsleep((void*) conditionVariable, lock);
}

void
cv_signal(cond_t* conditionVariable)
{
	threadwakeup((void*) conditionVariable);
}
