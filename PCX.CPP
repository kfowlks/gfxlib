#include "pcx.h"
#include <pc.h>
#include <stdio.h>


void Pcx_Loader::set_palette(int r,int g,int b,int entry)
{
   outportb(0x3c8,(BYTE)entry);
   outportb(0x3c9,(BYTE)r);
   outportb(0x3c9,(BYTE)g);
   outportb(0x3c9,(BYTE)b);
}


bool Pcx_Loader::Load_Pcx(char *filename,unsigned char *image,int pcx_width,int pcx_height)
{

FILE *fp;
int num_bytes,index;
long count=0;
unsigned char data;


char temp_buffer;

// Open the file
fp = fopen(filename,"rb");


for (index=0; index<128; index++)
    {
   temp_buffer = (char)getc(fp);
   } // end for index

count=0;

while(count <= pcx_width * pcx_height)
     {
     // get the first piece of data

     data = (unsigned char)getc(fp);

     // is this a rle?

     if (data>=192 && data<=255)
        {
        // how many bytes in run?

        num_bytes = data-192;

        // get the actual data for the run

        data  = (unsigned char)getc(fp);

        // replicate data in buffer num_bytes times

        while(num_bytes-->0)
             {
             image[count++] = data;

             } // end while

        } // end if rle
     else
        {
        // actual data, just copy it into buffer at next location

        image[count++] = data;

        } // end else not rle

     } // end while

fclose(fp);

return true;
}


// Load the pallete seprately (to save time in the long run)
bool Pcx_Loader::Load_Pal(char *filename)

{
  unsigned char red,green,blue;
  int index;

  FILE *fp;

  fp = fopen(filename,"rb");

  fseek(fp,-768L,SEEK_END);

  // load the pallete into the palette

  for (index=0; index<256; index++)
      {
    // get the red component

        red= (unsigned char)(getc(fp) >> 2);

    // get the green component

        green = (unsigned char)(getc(fp) >> 2);

    // get the blue component

        blue  = (unsigned char)(getc(fp) >> 2);

       set_palette(red,green,blue,index);
    } // end for index

fclose(fp);

return true;
}

