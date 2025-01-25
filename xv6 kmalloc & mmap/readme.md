# xv6 kmalloc and mmap

## Project Overview
This repository includes two main enhancements to the xv6 operating system:

1. **Kernel Memory Allocator (`kmalloc`)**:
   - Develop a kernel-level memory allocator capable of managing variable-sized chunks of memory without excessive waste.

2. **Anonymous Memory Mapping (`mmap`)**:
   - Implement an anonymous memory mapping system call and its complementary unmap system call (`munmap`).

---

## Part 4a: Kernel Memory Allocator (`kmalloc`)

### Features
1. **Memory Allocation**:
   - `kmalloc(uint nbytes)`: Allocates memory of the specified size.
   - `kmfree(void *addr)`: Frees previously allocated memory.

2. **Limitations**:
   - Allocations are limited to 4096 bytes (one page).
   - Memory is managed using the `kalloc()` function to acquire new pages.

3. **Testing**:
   - Introduced system calls to allow testing of `kmalloc` and `kmfree` from user space.

### Example Usage
```c
struct mystruct *s = kmalloc(sizeof(*s));
// Use the allocated memory...
kmfree(s);
```

### Compilation and Testing
- Compile xv6:
  ```bash
  make clean && make qemu-nox
  ```
- Run tests:
  ```bash
  ./test-kmalloc.sh
  ```

---

## Part 4b: Anonymous Memory Mapping (`mmap`)

### Features
1. **Memory Mapping**:
   - `mmap(void *addr, int length, int prot, int flags, int fd, int offset)`:
     - Maps a memory region into the process's address space.
     - Returns the starting address of the mapped region.
   - `munmap(void *addr, uint length)`:
     - Unmaps a previously mapped region and clears its memory.

2. **Data Structure**:
   - Each process maintains a linked list of memory mappings, tracking:
     - Starting address
     - Length
     - Region type (anonymous or file-backed)
     - Metadata like file descriptors and offsets (anonymous regions set fd to -1).

3. **Error Handling**:
   - Ensure valid inputs for `addr` and `length`.
   - Clean up allocated memory on errors to prevent leaks.

### Example Usage
```c
void *addr = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_ANONYMOUS, -1, 0);
// Use the mapped memory...
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
1. **For `kmalloc`**:
   - Adapt `umalloc.c` for kernel usage.
   - Ensure no memory requests exceed 4096 bytes.

2. **For `mmap`**:
   - Use `allocuvm()` for memory mapping.
   - Use `deallocuvm()` for unmapping.
   - Zero-out memory during mapping to prevent data leakage.
   - Free data structures in `freevm()` to avoid kernel memory leaks.

3. **Debugging**:
   - Add helper functions to print memory mappings for debugging.
   - Extensively test using both provided and custom test cases.

---

## Additional Resources
- [Programming Language in C (Kernighan & Ritchie)](https://ascslab.org/courses/ec413/reading/C-book.pdf)
- [Linux `mmap` Man Page](http://man7.org/linux/man-pages/man2/mmap.2.html)
- [OSTEP](http://www.ostep.org/)

---

## Notes
- Modify `Makefile` to include test files and new system calls.
- Use `Ctrl-A + X` to exit xv6 when running inside `qemu`.
