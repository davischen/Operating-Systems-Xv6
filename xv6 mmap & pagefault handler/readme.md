# mmap and pagefault handler

## Project Overview
This repository includes advanced enhancements to the xv6 operating system:

1. **Pagefault Handler**:
   - Implement lazy page allocation to allocate pages only upon access.

2. **File-backed `mmap`**:
   - Extend `mmap` to support file-backed regions and implement `msync` for persisting changes.

---

## Part 5a: Pagefault Handler

### Features
1. **Lazy Page Allocation**:
   - Modify `mmap` to avoid pre-allocating pages.
   - Allocate pages dynamically on pagefaults.

2. **Pagefault Handling**:
   - Add a pagefault handler in `trap.c`.
   - Verify if the faulting address belongs to a valid `mmap` region.
   - Allocate and map the necessary page upon validation.

3. **Debugging**:
   - Include detailed debug statements in the pagefault handler for troubleshooting.

### Debugging Example
```c
void pagefault_handler(struct trapframe *tf) {
    cprintf("============in pagefault_handler============\n");
    cprintf("pid %d %s: trap %d err %d on cpu %d eip 0x%x addr 0x%x\n",
            curproc->pid, curproc->name, tf->trapno, tf->err, cpuid(), tf->eip, fault_addr);
}
```

### Compilation and Testing
- Compile xv6:
  ```bash
  make clean && make qemu-nox
  ```
- Run tests:
  ```bash
  ./test-mmap.sh -c -v
  ```

---

## Part 5b: File-backed `mmap`

### Features
1. **File-backed Memory Mapping**:
   - Extend `mmap` to support file-backed regions.
   - Initialize mapped memory from a file descriptor.
   - Use `fileseek` to manage offsets in the file.

2. **System Calls**:
   - `mmap`: Map a memory region with support for file backing.
   - `munmap`: Free a mapped memory region and close associated file descriptors.
   - `msync`: Write changes in memory back to the file.

3. **Optimization**:
   - Check the dirty bit to write only modified pages during `msync`.
   - Write contiguous pages in chunks to minimize overhead.

### Example Usage
```c
void *addr = mmap(NULL, 4096, PROT_WRITE, MAP_FILE, fd, 0);
// Modify the mapped memory...
msync(addr, 4096);
munmap(addr, 4096);
```

### Compilation and Testing
- Compile xv6:
  ```bash
  make clean && make qemu-nox
  ```
- Run tests:
  ```bash
  ./test-mmap.sh -c -v
  ```

---

## Tips
1. **Pagefault Handler**:
   - Use `PGROUNDDOWN` to align faulting addresses to page boundaries.
   - Ensure proper cleanup in case of errors.

2. **File-backed `mmap`**:
   - Duplicate file descriptors to prevent premature closure.
   - Free resources during `munmap` to avoid memory leaks.

3. **Debugging**:
   - Add helper functions to print `mmap` regions and their metadata.
   - Use extensive logging for `msync` and pagefaults.

---

## Additional Resources
- [xv6 mmap Assignment](https://pdos.csail.mit.edu/6.828/2012/homework/xv6-zero-fill.html)
- [Linux `mmap` Man Page](http://man7.org/linux/man-pages/man2/mmap.2.html)
- [OSTEP](http://www.ostep.org/)

---

## Notes
- Modify `Makefile` to include new test files and system calls.
- Use `Ctrl-A + X` to exit xv6 when running inside `qemu`.
