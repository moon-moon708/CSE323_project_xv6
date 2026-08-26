#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "mailbox.h"
#include "syscall.h"

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

int
sys_ksend(void)
{
  int chan;
  char *buf;
  int len;

  if(argint(0, &chan) < 0)
    return -1;
  if(argint(2, &len) < 0)
    return -1;
  if(argptr(1, &buf, len) < 0)
    return -1;
  
  return mailboxsend(chan, buf, len);
}

int
sys_krecv(void)
{
  int chan;
  char *buf;
  int maxlen;

  if(argint(0, &chan) < 0)
    return -1;
  if(argint(2, &maxlen) < 0)
    return -1;
  if(argptr(1, &buf, maxlen) < 0)
    return -1;
  
  return mailboxrecv(chan, buf, maxlen);
}

int
sys_setpriority(void)
{
  int pid;
  int prio;

  if(argint(0, &pid) < 0)
    return -1;
  if(argint(1, &prio) < 0)
    return -1;
  
  return setprocpriority(pid, prio);
}
