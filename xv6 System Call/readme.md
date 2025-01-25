# xv6 System Call

## Project Overview
This repository contains two projects focused on operating systems concepts:

1. **Unix Shell Implementation**
   - Develop a simple Unix shell named `wish` with interactive and batch modes.
   - Implement essential functionalities, including command execution, redirection, and parallel commands.
   - Support built-in commands (`exit`, `cd`, and `path`).

2. **xv6 System Call Extension**
   - Add a new system call `getreadcount()` to the xv6 kernel.
   - Track how many times the `read()` system call has been invoked since kernel boot.

---

## Project #1: Unix Shell (`wish`)

### Features
1. **Interactive Mode**:
   - Users can execute commands interactively through a prompt (`wish>`).

2. **Batch Mode**:
   - Accepts a batch file containing commands to execute.

3. **Built-in Commands**:
   - `exit`: Terminates the shell.
   - `cd <path>`: Changes the current working directory.
   - `path <dir1> <dir2> ...`: Sets directories for searching executables.

4. **Redirection**:
   - Output redirection to a file using the `>` operator.

5. **Parallel Commands**:
   - Execute multiple commands in parallel using the `&` operator.

6. **Error Handling**:
   - All errors produce a single error message: `An error has occurred`.

### Compilation and Testing
- Add code to `wish.c`.
- Compile using the provided `Makefile`:
  ```bash
  make
  ```
- Run tests using:
  ```bash
  ./test-wish.sh
  ```

---

## Project #2: xv6 System Call (`getreadcount`)

### Objectives
1. Extend the xv6 kernel by implementing the `getreadcount()` system call.
   - Returns the total number of `read()` calls made by all processes.

### Steps
1. Understand xv6's system call mechanism.
2. Modify the kernel to include the new system call:
   - Update system call table.
   - Add the implementation for `getreadcount()`.
3. Rebuild the kernel and test the new system call.

### Environment Setup
1. Use Linux or set up a virtual machine with Ubuntu 18.04:
   - Install necessary tools:
     ```bash
     sudo apt-get install build-essential gawk qemu expect
     ```
2. Build and run tests:
   ```bash
   ./test-getreadcounts.sh
   ```

### Testing
- Test the implementation using `test-getreadcounts.sh`:
  ```bash
  ./test-getreadcounts.sh -s
  ```

---

## Additional Resources
- xv6 Reference Material:
  - [MIT xv6](https://pdos.csail.mit.edu/6.828/2017/xv6.html)
  - [OSTEP](http://www.ostep.org/)
- Professor Remzi Arpaci-Dusseau's Video Walkthrough:
  - [Video](https://www.youtube.com/watch?v=vR6z2QGcoo8)

---

## Notes
- Follow the structured approach to implement features step-by-step.
- Test extensively to ensure the functionality meets specifications.
- Use debugging tools like `gdb` for troubleshooting.

