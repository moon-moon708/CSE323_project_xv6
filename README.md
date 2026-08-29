Two kernel-level additions to the xv6-public teaching operating system:

Mailbox IPC — channel-based synchronous message passing between processes
Priority scheduling with aging — a priority-based scheduler that prevents starvation

Course project, Department of Electrical and Computer Engineering, North South University.

Requirements

Ubuntu (native or in a VM). Install the toolchain:

bash
sudo apt update
sudo apt install -y build-essential gcc-multilib qemu-system-x86 gdb
Build
bash
git clone <this-repo>
cd <this-repo>
make clean
make

A successful build ends with the filesystem image being written:

./mkfs fs.img README _cat _echo ... _mailboxtest2 _prioritytest
balloc: first 769 blocks have been allocated

Linker warnings about .note.GNU-stack or LOAD segment with RWX permissions are expected on recent binutils and can be ignored.

Run
bash
make qemu-nox CPUS=1

Wait for init: starting sh and the $ prompt. CPUS=1 keeps console output from interleaving between cores; drop it to run on the default 2 CPUs.

To exit QEMU: press Ctrl+A, release, then press x.

Running the tests

Three user programs are included. Run them from the xv6 shell.

mailboxtest2 — multi-channel IPC

Forks two children that exchange messages in opposite directions: child 1 sends on channel 1 and receives on channel 2, child 2 does the reverse.

$ mailboxtest2
=== Multi-Channel Test ===

Process 1 [PID 4]: Sending on channel 1
Process 1 [PID 4]: Receiving on channel 2
Process 2 [PID 5]: Sending on channel 2
Process 2 [PID 5]: Receiving on channel 1
Process 2 [PID 5]: Got: "Message on channel 1"
Process 1 [PID 4]: Got: "Message on channel 2"

=== Multi-Channel Test Complete ===

Both processes receive the correct payload with no deadlock and no message loss, which is the point of the test: the two channels stay isolated and the blocking handoff works in both directions at once.

mailboxtest — single-channel IPC

The simpler case: parent sends on channel 0, child blocks in krecv() until the message arrives.

prioritytest — priority scheduling and aging

Forks three children at priorities 1, 5, and 10, each doing five units of work.

$ prioritytest
========================================
Priority Scheduling with Aging Test
========================================
Creating 3 processes:
  Process 1: Priority 1 (lowest)
  Process 2: Priority 5 (medium)
  Process 3: Priority 10 (highest)
...
[PID 4] Priority 1: Finished
[PID 5] Priority 5: Finished
[PID 6] Priority 10: Finished

The priority-10 process is scheduled most often, and the priority-1 process still completes all five steps rather than starving — aging raised its effective priority while it waited.

## Video Demonstration

A video demonstration of the project, showing the Mailbox IPC and Priority Scheduling with Aging implementations in action on xv6, can be viewed here:

[video6095791446206454979.mp4](video6095791446206454979.mp4)

