// Channel-based message mailbox IPC implementation

#include "types.h"
#include "defs.h"
#include "param.h"
#include "mmu.h"
#include "proc.h"
#include "mailbox.h"

struct mailbox mailboxes[NCHAN];

// Copy len bytes from user address uva to kernel address dst.
// Returns 0 on success, -1 on failure.
static int
copyin(pde_t *pgdir, char *dst, uint uva, uint len)
{
  char *pa0;
  uint n, va0;

  while(len > 0){
    va0 = (uint)PGROUNDDOWN(uva);
    pa0 = uva2ka(pgdir, (char*)va0);
    if(pa0 == 0)
      return -1;
    n = PGSIZE - (uva - va0);
    if(n > len)
      n = len;
    memmove(dst, pa0 + (uva - va0), n);
    len -= n;
    dst += n;
    uva = va0 + PGSIZE;
  }
  return 0;
}

// Initialize all mailboxes
void
mailboxinit(void)
{
  int i;
  
  for(i = 0; i < NCHAN; i++){
    initlock(&mailboxes[i].lock, "mailbox");
    mailboxes[i].len = 0;
    mailboxes[i].full = 0;
  }
}

// Send a message to channel chan
// Returns number of bytes sent, or -1 on error
int
mailboxsend(int chan, char *buf, int len)
{
  struct mailbox *mb;
  struct proc *curproc;
  
  if(chan < 0 || chan >= NCHAN)
    return -1;
  if(len < 0 || len > MAXMSGSIZE)
    return -1;
  
  mb = &mailboxes[chan];
  curproc = myproc();
  
  acquire(&mb->lock);
  
  // Wait until channel is empty
  while(mb->full){
    if(curproc->killed){
      release(&mb->lock);
      return -1;
    }
    sleep(&mb->full, &mb->lock);
  }
  
  // Copy message from user space
  if(copyin(curproc->pgdir, mb->msg, (uint)buf, len) < 0){
    release(&mb->lock);
    return -1;
  }
  
  mb->len = len;
  mb->full = 1;
  
  // Wake up any processes waiting to receive
  wakeup(&mb->empty);
  
  release(&mb->lock);
  return len;
}

// Receive a message from channel chan
// Returns number of bytes received, or -1 on error
int
mailboxrecv(int chan, char *buf, int maxlen)
{
  struct mailbox *mb;
  struct proc *curproc;
  int len;
  
  if(chan < 0 || chan >= NCHAN)
    return -1;
  if(maxlen < 0)
    return -1;
  
  mb = &mailboxes[chan];
  curproc = myproc();
  
  acquire(&mb->lock);
  
  // Wait until channel has a message
  while(!mb->full){
    if(curproc->killed){
      release(&mb->lock);
      return -1;
    }
    sleep(&mb->empty, &mb->lock);
  }
  
  // Copy message to user space (up to maxlen)
  len = mb->len;
  if(len > maxlen)
    len = maxlen;
  
  if(copyout(curproc->pgdir, (uint)buf, mb->msg, len) < 0){
    release(&mb->lock);
    return -1;
  }
  
  // Clear mailbox
  mb->len = 0;
  mb->full = 0;
  
  // Wake up any processes waiting to send
  wakeup(&mb->full);
  
  release(&mb->lock);
  return len;
}

