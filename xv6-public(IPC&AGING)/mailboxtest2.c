#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
  int pid1, pid2;
  char msg1[] = "Message on channel 1";
  char msg2[] = "Message on channel 2";
  char recv1[256], recv2[256];
  int len1, len2;

  printf(1, "=== Multi-Channel Test ===\n\n");

  pid1 = fork();
  if(pid1 == 0){
    // Child 1: send on channel 1, receive on channel 2
    printf(1, "Process 1 [PID %d]: Sending on channel 1\n", getpid());
    ksend(1, msg1, strlen(msg1));
    
    printf(1, "Process 1 [PID %d]: Receiving on channel 2\n", getpid());
    len2 = krecv(2, recv2, sizeof(recv2));
    recv2[len2] = '\0';
    printf(1, "Process 1 [PID %d]: Got: \"%s\"\n", getpid(), recv2);
    exit();
  }

  pid2 = fork();
  if(pid2 == 0){
    // Child 2: send on channel 2, receive on channel 1
    printf(1, "Process 2 [PID %d]: Sending on channel 2\n", getpid());
    ksend(2, msg2, strlen(msg2));
    
    printf(1, "Process 2 [PID %d]: Receiving on channel 1\n", getpid());
    len1 = krecv(1, recv1, sizeof(recv1));
    recv1[len1] = '\0';
    printf(1, "Process 2 [PID %d]: Got: \"%s\"\n", getpid(), recv1);
    exit();
  }

  // Parent waits for both
  wait();
  wait();
  printf(1, "\n=== Multi-Channel Test Complete ===\n");
  exit();
}

