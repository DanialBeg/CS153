#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  int stat;
  argint(0, &stat);
  exit(stat);
  return 0;  // not reached
}

int
sys_setpriority(void)
{

}

int
sys_wait(void)
{
  int* stat;
  if(argptr(0, (void*)&stat, sizeof(*stat)) < 0)
	return -1;
  return wait(stat);
}

int
sys_waitpid(void)
{
   int pid;
   //int options = 0;
   argint(0, &pid);
   //if(argint(2, &options) < 0)
	//return -1;
   int* status;
   if(argptr(1, (void*) &status, sizeof(*status)) < 0)
	return -1;
   return waitpid(pid, status, 0);
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
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
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
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
   uint xticks;

   acquire(&tickslock);
   xticks = ticks;
   release(&tickslock);
   return xticks;
}
