// Channel-based message mailbox IPC

#ifndef _MAILBOX_H_
#define _MAILBOX_H_

#include "types.h"
#include "spinlock.h"

#define NCHAN 16        // Number of channels (0-15)
#define MAXMSGSIZE 256  // Maximum message size in bytes

struct mailbox {
  struct spinlock lock;  // Protects this mailbox
  char msg[MAXMSGSIZE];  // Message buffer
  int len;               // Message length (0 if empty)
  int full;              // 1 if message present, 0 if empty
  int empty;             // Used as wakeup channel for receivers
};

void mailboxinit(void);
int mailboxsend(int chan, char *buf, int len);
int mailboxrecv(int chan, char *buf, int maxlen);

#endif // _MAILBOX_H_

