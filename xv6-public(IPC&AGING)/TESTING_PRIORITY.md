# Testing Priority Scheduling with Aging

## Building xv6 with Priority Scheduling

1. **Compile xv6:**
   ```bash
   make clean
   make
   ```

2. **Run xv6 in QEMU:**
   ```bash
   make qemu
   ```

## Running the Test Suite

Once inside xv6, run the comprehensive test:

```bash
prioritytest
```

This will run three tests:
1. **Basic Priority Scheduling**: Creates processes with priorities 10, 5, and 1. Higher priority processes should run more frequently.
2. **Aging Mechanism**: Creates a high priority (10) and low priority (1) process. The low priority process should eventually run due to aging.
3. **setpriority System Call**: Tests the setpriority() function with valid and invalid inputs.

## Manual Testing

### Test 1: Verify Priority Scheduling

Create a simple test program or use the shell:

```bash
# In xv6 shell, you can create processes and set priorities
# (Note: You'll need to write a small program or use the test)
```

### Test 2: Verify Aging

Run multiple processes with different priorities and observe that low priority processes eventually get CPU time.

### Test 3: Test setpriority System Call

```bash
# From within a program:
setpriority(pid, priority);  // Set priority 0-10
```

## Expected Behavior

1. **Priority Scheduling:**
   - Processes with higher priority (closer to 10) should receive CPU time more frequently
   - When multiple processes are RUNNABLE, the scheduler should select the one with highest priority

2. **Aging:**
   - Processes that wait in RUNNABLE state for 10+ ticks should have their priority increased by 1
   - This prevents starvation - even low priority processes will eventually run

3. **setpriority:**
   - Should accept priorities in range 0-10
   - Should reject invalid priorities (return -1)
   - Should successfully update process priority

## Observing Results

Watch the console output to see:
- Which processes run and in what order
- How frequently each priority level gets CPU time
- Evidence of aging (low priority processes eventually running)

## Troubleshooting

If tests don't work as expected:
1. Verify the kernel compiled without errors
2. Check that all files were modified correctly
3. Ensure the test program compiled and is in the filesystem
4. Look for kernel panic messages or errors

