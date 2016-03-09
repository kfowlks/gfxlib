// Sprite lib for project code named Dextor
// Written by Kevin Fowlks
// Lead programmer / Sound SFX / Design
// Created March 19,1998


typedef struct pcx_header_type
        {
        char manufacturer;
        char version;
        char encoding;
        char bits_per_pixel;
        int x,y;
        int width,height;
        int horz_res;
        int vert_res;
        char ega_palette[48];
        char reserved;
        char num_color_planes;
        int bytes_per_line;
        int palette_type;
        char padding[58];
        } pcx_header,*ptr_pcx_header;


class Pcx_Loader
{

public:
#define BYTE unsigned char
pcx_header_type phead;
//Pcx_Loader(){};
bool Load_Pcx(char *,unsigned char *,int ,int);
bool Load_Pal(char *);
void set_palette(int ,int ,int ,int );

};






