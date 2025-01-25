//
// Kernel memory system calls.
// This is purely for testing purposes and is a wrapper for
// calling the kmalloc & kmfree from the user space.
//

#include "types.h"
#include "defs.h"

int
sys_kmalloc(void)
{
  int nbytes;

  if (argint(0, &nbytes) < 0) {
    return -1;
  }
  return (int)kmalloc((uint)nbytes);
}

int
sys_kmfree(void)
{
  void *addr;

  if (argptr(0, (void*)&addr, sizeof(addr)) < 0) {
    return -1;
  }
  kmfree(addr);
  return 0;
}
