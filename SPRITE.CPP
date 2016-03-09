#include <stdlib.h>
#include <stdio.h>
#include "pcx.h"
#include "sprite.h"



Sprite::Sprite(int spr_w,int spr_h,int n_frames)
{
  t=0;
  width=640;
  height=480;
  spr_width=spr_w;
  spr_height=spr_h;
  x=0;
  y=0;
  trans=255;
  num_frames=n_frames;
  num_waves=0;
  cur_frame=0;

  background_buffer=(unsigned char *)malloc(spr_h*spr_w);

  for (int uu=0;uu<n_frames;uu++)
  buffer[uu]=(unsigned char *)malloc(spr_h*spr_w);

}

bool Sprite::Load_Pcx(char *filename,int frame)
{

if (Pcx_Loader::Load_Pcx(filename,buffer[frame],spr_width,spr_height))
   return true;

return false;

}

bool Sprite::Hit(Sprite temp,int degree)
{


if ((temp.x > x+spr_width-degree) || (x > temp.x+temp.spr_width-degree) ||
    (temp.y > y+spr_width-degree) || (y > temp.y+temp.spr_width-degree))
    return false;
else
    return true;


}




void Sprite::Blit(unsigned char *dest)
{

/*  if (!VBE20)
  {
  unsigned char *tempptr= video_buffer;

   for(Bank=0;Bank<NumberOfBanks;Bank++)
     {SetBank(Bank);
      memmove(video_screen,tempptr,65536);
      tempptr+=65536;
     }   
   SetBank(Bank);
   if(MemAfterBanks)
     { memmove(video_screen,tempptr,MemAfterBanks);
     }
  return;
  }
*/


  int sprite_offset,h,work_offset=0;
  unsigned char *Temp_buffer;

//  work_offset=0;
  Temp_buffer=background_buffer;

  sprite_offset=(y<<9)+(y<<7)+x;

for (h=0;h<spr_height;h++)
    {
     memcpy((unsigned char *)&dest[sprite_offset],
     (unsigned char *)&Temp_buffer[work_offset] ,spr_width);
     sprite_offset+=width;
     work_offset+=spr_width;
    }
}


void Sprite::Get_BackGrd(unsigned char *dest)
{

  int sprite_offset,h,work_offset=0;
  //work_offset=0;
  sprite_offset=(y<<9)+(y<<7)+x;

for (h=0; h < spr_height;h++)
    {
     memcpy((unsigned char *)&background_buffer[work_offset],
     (unsigned char *)&dest[sprite_offset] ,spr_width);
     sprite_offset+=width;
     work_offset+=spr_width;
    }
}



void Sprite::Blit_Trans(unsigned char *dest)
{
	int xindex, yindex,sprite_offset,work_offset;
        unsigned char *Temp_buffer;
        sprite_offset=(y<<9)+(y<<7)+x;

        Temp_buffer=buffer[cur_frame];
        work_offset=0;

	for (yindex = 0; yindex < spr_height; yindex++) {
		for (xindex = 0; xindex < spr_width; xindex++) {
			if (int(Temp_buffer[xindex]) != trans) {
                        dest[sprite_offset+xindex]=Temp_buffer[xindex];

                        }
		}
		sprite_offset += width; /* next line of video buffer */
		Temp_buffer += spr_width; /* next line of bitmap */
	}
}






void Sprite::Draw(unsigned char *dest)
{
  Get_BackGrd(dest);
  Blit_Trans(dest);
//  clip_sprite_trans(spr_dumb,dest,0);
  }




void Sprite::Erase(unsigned char *dest)
{
  Blit(dest);
}



Sprite::~Sprite()
{
  free(background_buffer);
  for (int t=0;t<num_frames;t++)
      free(buffer[t]);

  //    cout << trans <<endl;
}
