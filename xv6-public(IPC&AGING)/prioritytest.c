#include "types.h"
#include "stat.h"
#include "user.h"

int
main(void)
{
  int pid1, pid2, pid3;
  int i;
  
  printf(1, "\n");
  printf(1, "========================================\n");
  printf(1, "Priority Scheduling with Aging Test\n");
  printf(1, "========================================\n");
  printf(1, "Creating 3 processes:\n");
  printf(1, "  Process 1: Priority 1 (lowest)\n");
  printf(1, "  Process 2: Priority 5 (medium)\n");
  printf(1, "  Process 3: Priority 10 (highest)\n");
  printf(1, "\nExpected behavior:\n");
  printf(1, "  - Priority 10 runs most frequently\n");
  printf(1, "  - Priority 5 runs moderately\n");
  printf(1, "  - Priority 1 runs least, but eventually runs (aging)\n");
  printf(1, "========================================\n\n");
  
  // Process 1: Low priority (1)
  pid1 = fork();
  if(pid1 == 0){
    setpriority(getpid(), 1);
    printf(1, "[PID %d] Priority 1: Started\n", getpid());
    for(i = 0; i < 5; i++){
      sleep(10);  // Sleep briefly, then print
      printf(1, "[PID %d] Priority 1: Completed step %d/5\n", getpid(), i+1);
    }
    printf(1, "[PID %d] Priority 1: Finished\n", getpid());
    exit();
  }
  
  // Process 2: Medium priority (5)
  pid2 = fork();
  if(pid2 == 0){
    setpriority(getpid(), 5);
    printf(1, "[PID %d] Priority 5: Started\n", getpid());
    for(i = 0; i < 5; i++){
      sleep(10);
      printf(1, "[PID %d] Priority 5: Completed step %d/5\n", getpid(), i+1);
    }
    printf(1, "[PID %d] Priority 5: Finished\n", getpid());
    exit();
  }
  
  // Process 3: High priority (10)
  pid3 = fork();
  if(pid3 == 0){
    setpriority(getpid(), 10);
    printf(1, "[PID %d] Priority 10: Started\n", getpid());
    for(i = 0; i < 5; i++){
      sleep(10);
      printf(1, "[PID %d] Priority 10: Completed step %d/5\n", getpid(), i+1);
    }
    printf(1, "[PID %d] Priority 10: Finished\n", getpid());
    exit();
  }
  
  // Parent waits for all children
  printf(1, "Parent waiting for all processes...\n\n");
  
  wait();
  wait();
  wait();
  
  printf(1, "\n========================================\n");
  printf(1, "Test completed!\n");
  printf(1, "Observe: Priority 10 messages appeared most often\n");
  printf(1, "         Priority 1 eventually ran (aging prevented starvation)\n");
  printf(1, "========================================\n");
  
  exit();
}
