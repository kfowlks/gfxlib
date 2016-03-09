// Written by Kevin Fowlks
// Lead programmer / Sound SFX / Design
// Created March 19,1998


// Defines and stuff
// Note: theses are GLOBAL


typedef struct {
  unsigned ModeAttributes;
  unsigned granularity,startseg,farfunc;
  short bscanline;
  short XResolution;
  short YResolution;
  short charpixels;
  unsigned bogus1,bogus2,bogus3,bogus4;
  unsigned PhysBasePtr;
  char bogus[228];
} ModeInfoBlock;



class svgalib {


ModeInfoBlock *get_mode_info(int mode);
void make_selector(void);

public:
#define BYTE unsigned char
#define vesa_640x480x256 0x101
#define vesa_800x600x256 0x103
#define vesa_1024x768x256 0x105
#define vesa_1280x1024x256 0x107

#define WORD unsigned short int
#define DWORD unsigned int
#define BOOL signed char


// Global Varibles
unsigned int ADDR;
int width,height;
int video_buffer;
int video_mode;
int curbank;

bool VBE20;

ModeInfoBlock *mb;

BYTE fonttable[128][16];
unsigned char font_color;

unsigned char *double_buffer;

svgalib(int);
void set_palette(int ,int ,int ,int);
void pixel(unsigned int ,unsigned int ,unsigned int);
void clear_buffer(unsigned char *,unsigned char ,int);
void buffer_pixel(unsigned int , unsigned int, unsigned char);
BYTE buffer_getpixel(unsigned int ,unsigned int );
BYTE get_pixel(int , int );
void show_buffer(unsigned char *);
void wait_for_vsync(void);
void line(int , int , int , int , char );
WORD font_readtable(void);
BOOL font_putchar(int x, int y, char ch);
void font_putstring(int x, int y, char *str);
void setbank_NOVBE20(int);
void pixel_NOVBE20(short , short , char );
int  init_NOVBE20(unsigned short );

~svgalib();


};


