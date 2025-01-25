# README

## Project Overview
This repository contains two projects that involve extending the xv6 operating system:

1. **Adding NULL Pages to xv6 Address Spaces**
   - Implement support for trapping null pointer dereferences.
   - Add the ability to change page protection levels for parts of a process’s address space.

2. **xv6 Kernel Threads**
   - Add kernel thread support by implementing `clone()` and `join()` system calls.
   - Develop a user-level thread library with thread creation, joining, and locks.

---

## Project #1: Adding NULL Pages to xv6 Address Spaces

### Features
1. **Null Pointer Dereference Handling**:
   - Modify xv6 to cause an exception on null pointer dereferences.
   - Update the Makefile to relocate the entry point to prevent execution from the NULL page.

2. **Read-Only Code**:
   - Introduce `mprotect` and `munprotect` system calls to change memory protection levels.
   - Ensure protections are inherited by child processes upon `fork()`.

3. **System Call Details**:
   - `int mprotect(void *addr, int len)`: Marks a region as read-only.
   - `int munprotect(void *addr, int len)`: Reverts a region to read-write.
   - Error handling for misaligned or invalid inputs.

4. **Trap Handling**:
   - xv6 traps and kills the process when:
     - A null pointer is dereferenced.
     - A write is attempted to an `mprotect`-ed region.

### Compilation and Testing
- Run tests using:
  ```bash
  ./test-null-pages.sh
  ```
- Compile xv6 with:
  ```bash
  make clean && make qemu-nox
  ```

---

## Project #2: xv6 Kernel Threads

### Features
1. **Kernel Thread System Calls**:
   - `int clone(void (*fcn)(void*, void*), void* arg1, void* arg2, void* stack)`:
     Creates a kernel thread sharing the parent’s address space.
   - `int join(void **stack)`:
     Waits for a child thread and retrieves its stack address.

2. **User-Level Thread Library**:
   - `int thread_create(void (*start_routine)(void *, void *), void *arg1, void *arg2)`:
     Allocates a new user stack and calls `clone()` to create a thread.
   - `int thread_join()`: Calls `join()` and frees the thread’s stack.
   - Lock functions:
     - `void lock_acquire(lock_t *)`
     - `void lock_release(lock_t *)`
     - `void lock_init(lock_t *)`

3. **Address Space Growth in Multi-threaded Processes**:
   - Add appropriate locks and updates to safely grow the address space.

4. **x86 Calling Convention**:
   - Ensure proper setup of user stacks for new threads, adhering to x86 conventions.

### Compilation and Testing
- Run tests using:
  ```bash
  ./test-threads.sh -c -v
  ```
- Compile xv6 with:
  ```bash
  make clean && make qemu-nox
  ```
- Add your own test cases to extensively verify the implementation.

---

## Additional Resources
- xv6 Documentation:
  - [MIT xv6](https://pdos.csail.mit.edu/6.828/2017/xv6.html)
  - [OSTEP](http://www.ostep.org/)
- Thread Locks:
  - [Threads and Locks](http://pages.cs.wisc.edu/~remzi/OSTEP/threads-locks.pdf)
- x86 Assembly:
  - [Programming Ground Up](https://download-mirror.savannah.gnu.org/releases/pgubook/ProgrammingGroundUp-1-0-booksize.pdf)

---

## Notes
- Follow a structured approach to implement features step-by-step.
- Use debugging tools like `gdb` and extensive logging to troubleshoot.
- Test with hidden cases to ensure robustness.
