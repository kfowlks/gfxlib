#include <stdlib.h>

class Memory {

unsigned char *buffer;
int Total_Mem_Used;
int Last_Buffer_Size;

public:
Memory():Total_Mem_Used(0),Last_Buffer_Size(0){};
Memory(int);
unsigned char *Ptr();
int Used_Mem();
~Memory(){free(buffer);}
};
