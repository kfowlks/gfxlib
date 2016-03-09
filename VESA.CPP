#include "vesa.h"
#include <iostream.h>
#include <pc.h>
#include <dpmi.h>
#include <go32.h>
#include <dos.h>
#include <string.h>
#include <sys/farptr.h>
#include <sys/movedata.h>
#include <sys/segments.h>


ModeInfoBlock *svgalib::get_mode_info(int mode)
{
	static ModeInfoBlock info;
	__dpmi_regs r;

	/* Use the transfer buffer to store the results of VBE call */
	r.x.ax = 0x4F01;
	r.x.cx = mode;
	r.x.es = __tb / 16;
	r.x.di = 0;
	__dpmi_int(0x10, &r);
	if(r.h.ah) return 0;
	dosmemget(__tb, sizeof(ModeInfoBlock), &info);
	return &info;
}


svgalib::svgalib(int mode):video_mode(mode),font_color(112),curbank(0)
{
	__dpmi_meminfo info;
	__dpmi_regs reg;

        VBE20=true;

	if (video_mode==NULL)
        video_mode=vesa_640x480x256;

        mb=get_mode_info(video_mode);



	if (!mb) {
          cout << "Get VESA mode info failed."<< endl;
         exit(1);
	}

        if (!(mb->ModeAttributes & 0x80)) {

        cout << "Linear frame buffer not supported for VESA mode." <<endl;
        VBE20=false;
        if (mode==vesa_640x480x256)
        init_NOVBE20(0x101);
        return;

	}



	width=mb->XResolution;
	height=mb->YResolution;


	info.size=width*height;
	info.address=mb->PhysBasePtr;
	if(__dpmi_physical_address_mapping(&info) == -1) {
        cout << "Physical mapping of address " << mb->PhysBasePtr << "failed!\n";
          exit(2);
	}
	ADDR=info.address;		/* Updated by above call */

        cout << endl << "Starting screen mode: " <<mb->XResolution << "x"<< mb->YResolution <<endl;
         delay (1000);

	reg.x.ax=0x4f02;

  switch (video_mode) {
        case vesa_640x480x256:
  	reg.x.bx=0x4101; // mode plus linear enable bit
          break;
        case vesa_800x600x256:
        reg.x.bx=0x4103; 	// mode plus linear enable bit
           break;
        case vesa_1024x768x256:
        reg.x.bx=0x4105; 	// mode plus linear enable bit
          break;
        case vesa_1280x1024x256:
	reg.x.bx=0x4107; 	// mode plus linear enable bit
          break;      }

	__dpmi_int(0x10,&reg);		/* set the mode */
	if(reg.h.al != 0x4f || reg.h.ah) {
          cout << "Mode set failed!"<<endl;
         exit(3);
	}
        make_selector();
}

void svgalib::make_selector(void)
 {

   video_buffer = __dpmi_allocate_ldt_descriptors(1);
   __dpmi_set_segment_base_address(video_buffer,ADDR);
   ADDR = 0;	/* basis now zero from selector */
   __dpmi_set_segment_limit(video_buffer,(width*height)|0xfff);

  //Enable videobuffer only needs to be called once
 _farpokeb(video_buffer,0,0);  //sets pixel so other pixels can be set
  return;

}

// Graphics stuff 

void svgalib::set_palette(int r,int g,int b,int entry)
{
   outportb(0x3c8,(BYTE)entry);
   outportb(0x3c9,(BYTE)r);
   outportb(0x3c9,(BYTE)g);
   outportb(0x3c9,(BYTE)b);
}

void svgalib::pixel(unsigned int x,unsigned int y,unsigned int color)
{

  if (!VBE20)
  {
        pixel_NOVBE20(x, y, color);
        return;
  }

  switch (video_mode)     {
         case vesa_640x480x256:
         _farnspokeb(ADDR+(y<<9)+(y<<7)+x,color);
         break;

         case vesa_800x600x256:
         _farnspokel(ADDR+((800*y))+x,color);
         break;

         case vesa_1024x768x256:
         _farnspokeb(ADDR+(y<<9)+(y<<9)+x,color);
         break;

         case vesa_1280x1024x256:
         _farnspokeb(ADDR+(y<<8)+(y<<10)+x,color);
         break;
                          }
}

void svgalib::clear_buffer(unsigned char *buffer,unsigned char value,int size)
{
    memset((unsigned char *)buffer,(int)value,size);
}


void svgalib::buffer_pixel(unsigned int x, unsigned int y, unsigned char color)
{

  switch (video_mode)  {

        case vesa_640x480x256:
        double_buffer[(y<<9)+(y<<7)+x]=color;
        break;

        case vesa_800x600x256:
        double_buffer[(800*y)+x]=color;
        break;

        case vesa_1024x768x256:
        double_buffer[(y<<9)+(y<<9)+x]=color;
        break;

        case vesa_1280x1024x256:
        double_buffer[(y<<8)+(y<<10)+x]=color;
        break;
                     }
}


BYTE svgalib::buffer_getpixel(unsigned int x,unsigned int y)
 {

  switch (video_mode)  {

        case vesa_640x480x256:
        return double_buffer[(y<<9)+(y<<7)+x];
        break;

        case vesa_800x600x256:
        return double_buffer[(800*y)+x];
        break;

        case vesa_1024x768x256:
        return double_buffer[(y<<9)+(y<<9)+x];
        break;

        case vesa_1280x1024x256:
        return double_buffer[(y<<8)+(y<<10)+x];
        break;
                     }
}

void svgalib::show_buffer(unsigned char *buffer)
{
        _movedatal(_my_ds(),(unsigned int )buffer,video_buffer,0,(width*height)/4);
}




BYTE svgalib::get_pixel(int x, int y) {

  switch (video_mode)           {

         case vesa_640x480x256:
         return _farnspeekb(ADDR+(y<<9)+(y<<7)+x);
         break;

         case vesa_800x600x256:
         _farnspeekb(ADDR+(800*y)+x);
         break;

         case vesa_1024x768x256:
         _farnspeekb(ADDR+(1024*y)+x);
         break;

         case vesa_1280x1024x256:
         _farnspeekb(ADDR+(y<<8)+(y<<10)+x);
         break;
                                }

}


void svgalib::wait_for_vsync(void)
{
	while (inportb(0x3da) & 0x08); /* vga is in retrace */
	while (!(inportb(0x3da) & 0x08)); /* wait for start of retrace */
}


void svgalib::line(int x1, int y1, int x2, int y2, char color)
{
        short int dx, dy, sdx, sdy, x, y, px, py;

        dx = x2 - x1;
        dy = y2 - y1;

        sdx = (dx < 0) ? -1 : 1;
        sdy = (dy < 0) ? -1 : 1;

        dx = sdx * dx + 1;
        dy = sdy * dy + 1;

        x = y = 0;

        px = x1;
        py = y1;

        if (dx >= dy) { 
                for (x = 0; x < dx; x++) {
                        pixel(px,py,color);
                        y += dy;
                        if (y >= dx) {
                                y -= dx;
                                py += sdy;
                        }
                        px += sdx;
                }
        } else { 
                for (y = 0; y < dy; y++) {
                        
                        pixel(px,py,color);
                        x += dx;
                        if (x >= dy) {
                                x -= dy;
                                px += sdx;
                        }
                        py += sdy;
                }
        }
}


svgalib::~svgalib()
{
	__dpmi_regs reg;
	reg.x.ax = 0x0003;
	__dpmi_int(0x10,&reg);
        __dpmi_free_ldt_descriptor(video_buffer);
}



// Read the font for text mode using BIOS
// 'return'(out): bytes/character of on-screen font
WORD svgalib::font_readtable(void)
{
   __dpmi_regs regs;

   regs.x.ax=0x1130;
   regs.h.bh=0x06;   // 8x16 font (MCGA, VGA)
   __dpmi_int(0x10, &regs);
   dosmemget((regs.x.es<<4)+regs.x.bp, sizeof(fonttable), fonttable);
   return(regs.x.cx);
}

// Draw a character on the screen
// 'x'(in): x position where draw the character
// 'y'(in): y position where draw the character
// 'ch'(in): the character to draw (0<=ch<=127)
// 'return'(out): TRUE if error
BOOL svgalib::font_putchar(int x, int y, char ch)
{
   int countx, county, cmp;

   for(county=0;county<16;county++)
      for(countx=0,cmp=0x80;countx<8;countx++,cmp>>=1)
         if(fonttable[ch][county]&cmp)
            buffer_pixel(x+countx, y+county, font_color);
   return false;
}



// Draw a string on the screen
// 'x'(in): x position where start to draw the string
// 'y'(in): y position where start to draw the string
// 'str'(in): the string to draw
// 'return'(out): TRUE if error
void svgalib::font_putstring(int x, int y, char *str)
{
   int i;

   for(i=0;str[i];i++)
   {
      font_putchar(x, y, str[i]);
      x+=8;
   }
   return;

}

//void svgalib::setbank(int bank)
void svgalib::pixel_NOVBE20(short x, short y, char c)
{
  int addr = y * mb->bscanline + x;
  setbank_NOVBE20((addr >> 16));
  _farpokeb(_dos_ds,0xA0000 + (addr & 0xFFFF), c);
  return;
}


void svgalib::setbank_NOVBE20(int bank)
{
  __dpmi_regs regs;
  
  if (bank == curbank) return;  
  curbank = bank;     
  bank <<= mb->granularity;
  regs.x.ax = 0x4F05;
  regs.x.bx = 0;
  regs.x.dx = bank;
  __dpmi_int(0x10, &regs);
  regs.x.ax = 0x4F05;
  regs.x.bx = 1;
  regs.x.dx = bank;
  __dpmi_int(0x10, &regs);
  return;
}

//void svgalib::setbank(int bank)
//void svgalib::pixel_NOVBE20(short x, short y, char c)

int svgalib::init_NOVBE20(unsigned short m)
{
  __dpmi_regs regs;
  regs.x.ax = 0x4F02;
  regs.x.bx = m;
  __dpmi_int(0x10, &regs);
  if(regs.x.ax == 0x4F)
  {return 1;}
  else
  {return 0;}
}

