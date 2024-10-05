#include <new> 
#include <stdlib.h> 
#include "memory.h"

namespace snp
{




// TODO Student
void *Memory::_new(size_t size) 
{ 
  (void) size;
  return malloc(size);
} 

// TODO Student
void Memory::_delete(void * p) 
{ 
  (void) p;
  free(p);
} 





}
