#include "types.h"
#include "stat.h"
#include "defs.h"
#include "param.h"

typedef long Align;

union header {
  struct {
    union header *ptr;
    uint size;
  } s;
  Align x;
};

typedef union header Header;

static Header base;
static Header *freep;

void
kmfree(void *ap)
{
  Header *bp, *p;
  //get header point
  bp = (Header*)ap - 1;
  //find the appropriate position to insert the newly freed block
  for(p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
    if(p >= p->s.ptr && (bp > p || bp < p->s.ptr))
      break;
      
  if(bp + bp->s.size == p->s.ptr){
    bp->s.size += p->s.ptr->s.size;
    bp->s.ptr = p->s.ptr->s.ptr;
  } else
    bp->s.ptr = p->s.ptr;
  if(p + p->s.size == bp){
    p->s.size += bp->s.size;
    p->s.ptr = bp->s.ptr;
  } else
    p->s.ptr = bp;
  freep = p;
}

static Header*
morecore()
{
  //cprintf("request more free memory\n");
  char *p;
  Header *hp;

  p = kalloc();
  if(p == 0)
    return 0;
  hp = (Header*)p;
  hp->s.size = 4096 / sizeof(Header); //kalloc always allocates 4096 bytes, and then takes Header sized chunks
  kmfree((void*)(hp + 1));
  return freep;
}
//allocates nbytes bytes of memory and returns a pointer to the start of the allocated block
void*
kmalloc(uint nbytes)
{
  Header *p, *prevp;
  uint nunits;
  //Check if the requested allocation size is greater than the maximum allowed size of 4096 bytes.
  if(nbytes > 4096)
  {
    panic("kmalloc: requested more than allowed in a single allocation");
  }
  
  //Calculate the number of Header units needed to satisfy the requested allocation size. 
  nunits = (nbytes + sizeof(Header) - 1)/sizeof(Header) + 1;
  //Check if the free list is empty. 
  if((prevp = freep) == 0){
    base.s.ptr = freep = prevp = &base;
    base.s.size = 0;
  }
  //Traverse the free list starting from freep 
  //until we find a block that is large enough to satisfy the allocation request.
  for(p = prevp->s.ptr; ; prevp = p, p = p->s.ptr){
    if(p->s.size >= nunits){
      //If the current block p is exactly the size we need
      if(p->s.size == nunits)
        prevp->s.ptr = p->s.ptr;
      else {
        //If the current block p is larger than the size we need, split it into two blocks
        p->s.size -= nunits;
        p += p->s.size;
        p->s.size = nunits;
      }
      //Update the freep pointer to point to the previous block prevp, 
      //which is now the newly allocated block.
      freep = prevp;
      return (void*)(p + 1);
    }
    // Check if the free list is empty.
    if(p == freep)
      if((p = morecore()) == 0)
        return 0;
  }
}