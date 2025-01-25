# Operating-Systems-Xv6

## xv6 Projects Overview
This repository contains four key projects enhancing the xv6 operating system:

1. [**xv6 kmalloc & mmap**](#xv6-kmalloc--mmap)
2. [**xv6 mmap & pagefault handler**](#xv6-mmap--pagefault-handler)
3. [**xv6 null page & thread**](#xv6-null-page--thread)
4. [**xv6 System Call**](#xv6-system-call)

---

## xv6 kmalloc & mmap

### Features
1. **Kernel Memory Allocator (`kmalloc`)**:
   - Adds `kmalloc` and `kmfree` for efficient kernel memory allocation.
   - Limits allocation to 4096 bytes per request.
   - Prevents wasteful memory allocation via `kalloc`.

2. **Anonymous Memory Mapping (`mmap`)**:
   - Implements `mmap` and `munmap` for memory mapping in user processes.
   - Supports page-aligned mappings and proper memory cleanup.

### Example Usage
```c
struct mystruct *s = kmalloc(sizeof(*s));
kmfree(s);
void *addr = mmap(NULL, 4096, PROT_READ, MAP_ANONYMOUS, -1, 0);
munmap(addr, 4096);
```

---

## xv6 mmap & pagefault handler

### Features
1. **Lazy Page Allocation**:
   - Modifies `mmap` to allocate pages on access.
   - Handles pagefaults dynamically.

2. **Pagefault Handler**:
   - Verifies faulting addresses and maps pages dynamically in `trap.c`.
   - Includes debugging support for effective troubleshooting.

3. **File-backed `mmap`**:
   - Supports memory mappings initialized from file contents.
   - Introduces `msync` for synchronizing memory changes to files.

### Example Usage
```c
void *addr = mmap(NULL, 4096, PROT_WRITE, MAP_FILE, fd, 0);
msync(addr, 4096);
munmap(addr, 4096);
```

---

## xv6 null page & thread

### Features
1. **Null Page Handling**:
   - Causes an exception on null pointer dereferences.
   - Modifies `Makefile` to adjust entry points for user programs.

2. **Thread Library**:
   - Implements `clone` and `join` system calls for kernel threads.
   - Provides `thread_create` and `thread_join` in user-space.
   - Includes ticket locks (`lock_acquire`/`lock_release`) for synchronization.

### Example Usage
```c
int thread_create(void (*start_routine)(void *, void *), void *arg1, void *arg2);
int thread_join();
```

---

## xv6 System Call

### Features
1. **New System Calls**:
   - Adds custom system calls such as `getreadcount`.
   - Enhances kernel functionalities for user-space testing.

2. **Testing Framework**:
   - Supports stress testing of system calls.
   - Provides example scripts and debugging tips.

### Example Usage
```c
int read_count = getreadcount();
printf("Number of reads: %d\n", read_count);
```
