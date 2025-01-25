#include "param.h"
#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"
#include "syscall.h"
#include "traps.h"
#include "memlayout.h"
#include "mmu.h"

#define MAX_SIZE (1024 * 1024) // 1MB

int main() {
    char *addr = (char*)0x4000;
    // allocate memory using mmap
    int count = 1;
    void *ptr;
    
    /*ptr = mmap(0,MAX_SIZE, 0, 0, -1, 0);
    if (ptr < 0) {
        printf(1, "XV6_TEST_OUTPUT : mmap failed\n");
    }
    memset(ptr, 0, 0);
        
    if (munmap(0, MAX_SIZE) < 0) {
        printf(1, "XV6_TEST_OUTPUT : munmap failed\n");
    }*/

    while (count<10) {
        int size=MAX_SIZE*count;
        ptr = mmap(addr,size, 0, 0, -1, 0);
        if (ptr == 0) {
            printf(1, "XV6_TEST_OUTPUT : mmap failed\n");
            break;
        }
        
        memset(ptr, 0, size);
        
        int pid = fork();

        if (pid < 0) {
            printf(1, "XV6_TEST_OUTPUT: fork failed\n");
        }
        if (pid == 0) {
            exit();
        }
        wait();

        if (munmap(ptr, size) < 0) {
            printf(1, "XV6_TEST_OUTPUT : munmap failed\n");
            break;
        }
        
        count++;
        printf(1,"Allocated and deallocated block %d\n", count);
        //sleep(1); // slow down loop for better observation
    }
    
    printf(1,"Allocating and deallocating %d blocks\n", count);
    
    printf(1,"mmap test successful!\n");
    return 0;
}