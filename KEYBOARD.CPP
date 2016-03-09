
 //
 //
 //
 // INPUT READING FUNCTIONS
 //
 //
 //
#include "keyboard.h"
#include <mem.h>
#include <conio.h>
#include <iostream.h>
Keyboard::Keyboard()
 {key_table = new unsigned char[84];//was 129
  if(key_table ==NULL)
  {ERROR(1);
  }

  for(int k=0;k<84;k++)//change to loop for sure?
  {key_table[k]=0;
  }
}
 
Keyboard::~Keyboard()
 { if(key_table != NULL){delete []key_table;}
 }
 
void Keyboard::ERROR(int ErrorNumber)
{switch(ErrorNumber)
 {case 1:break;
 }
} 
 
