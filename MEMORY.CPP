//#include <stdlib.h>
#include "memory.h"


Memory::Memory(int size)
{
 Last_Buffer_Size=size;
 buffer=(unsigned char *)malloc(size);
 Total_Mem_Used=+Last_Buffer_Size;
}

int Memory::Used_Mem()
{
  return Total_Mem_Used;
}

unsigned char *Memory::Ptr()
{
 return buffer;
}

