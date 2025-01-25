#include "param.h"
#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"
#include "syscall.h"
#include "traps.h"
#include "memlayout.h"


#define TEST_COUNT 20
#define MAX_SIZE 1000

int main()
{
    printf(1,"Testing kmfree...\n");

    void* ptrs[TEST_COUNT];

    // Allocate memory
    for (int i = 0; i < TEST_COUNT; i++)
    {
        ptrs[i] = kmalloc(MAX_SIZE);

        if (ptrs[i] == 0)
        {
            printf(1,"kmalloc failed to allocate memory!\n");
            return 0;
        }

        printf(1,"Allocated 1000 bytes at address %p\n", ptrs[i]);
    }

    // Free memory
    for (int i = 0; i < TEST_COUNT; i++)
    {
        kmfree(ptrs[i]);
        printf(1,"Freed memory at address %p\n", ptrs[i]);
    }

    printf(1,"kmfree test successful!\n");

    return 0;
}