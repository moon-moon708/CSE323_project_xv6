#include "types.h"
#include "stat.h"
#include "user.h"

#define CHAN 0

int
main(int argc, char *argv[])
{
  int pid;
  char send_msg[] = "Hello from parent process!";
  char recv_msg[256];
  int len;

  printf(1, "=== Mailbox IPC Test ===\n\n");

  pid = fork();
  
  if(pid < 0){
    printf(1, "fork failed\n");
    exit();
  }
  
  if(pid == 0){
    // Child process: receive message
    printf(1, "Child [PID %d]: Waiting to receive message on channel %d...\n", getpid(), CHAN);
    
    len = krecv(CHAN, recv_msg, sizeof(recv_msg));
    
    if(len < 0){
      printf(1, "Child: krecv failed\n");
      exit();
    }
    
    recv_msg[len] = '\0';  // Null terminate
    printf(1, "Child [PID %d]: Received %d bytes: \"%s\"\n", getpid(), len, recv_msg);
    printf(1, "Child [PID %d]: Exiting\n", getpid());
    exit();
  } else {
    // Parent process: send message
    printf(1, "Parent [PID %d]: Sending message on channel %d...\n", getpid(), CHAN);
    printf(1, "Parent [PID %d]: Message: \"%s\"\n", getpid(), send_msg);
    
    len = ksend(CHAN, send_msg, strlen(send_msg));
    
    if(len < 0){
      printf(1, "Parent: ksend failed\n");
      exit();
    }
    
    printf(1, "Parent [PID %d]: Sent %d bytes successfully\n", getpid(), len);
    printf(1, "Parent [PID %d]: Waiting for child to finish...\n", getpid());
    wait();
    printf(1, "\n=== Test Complete: IPC Working! ===\n");
    exit();
  }
}

