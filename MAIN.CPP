// Standard System Libs
#include <iostream.h>
#include <stdio.h>
#include <pc.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

// Custom Libs
#include "vesa.h"
#include "pcx.h"
#include "sprite.h"
#include "memory.h"
#include "kbinput.h"

#ifndef SBSOUND
#include <unistd.h>
#include "sb16.h"
#endif

// My Own Game in C++ using DJGPP and my own gfx lib
// Created By Kevin Fowlks
// Sample Game with Sound Routes borrowed from Inverse Reality Tut-7

svgalib	my_video(vesa_640x480x256);

Pcx_Loader my_pcx;

Sprite my_spr(64,64,3);
Sprite my_spr2(52,52,3);
Sprite my_fire(92,92,7);

Sprite my_rockets2(24,24,2);
Sprite my_rockets(24,24,2);
Sprite my_rocks(44,44,2);


unsigned char *buffer;


struct Rocks
{
       int x;
       int y;
       bool hit;
       int type;
       int speed;
};

// easy function animation etc..
void Init_Rocks(void);
void Destroy_Rocks(void);
void Create_Rocks(void);
void Ship_Anim(void);
void Erase_Ship_Anim(void);
void do_fire(void);
void Ship_Anim2(void);
void Erase_Ship_Anim2(void);
void Rocket_Animation(void);
void Rocket_Animation2(void);
void Erase_Rocket_Animation(void);
void Erase_Rocket_Animation2(void);

// Global declaration

#define	num_rocks 8


int r_dead=0;
int start_timer=0,frame_counter=0;
int my_timer=0;
//bool SOUND_ON=false;
bool rocket_flag=false;
bool rocket_flag2=false;
bool shipflag=false;
bool flagfire=false;
bool shipflag2=true;

Rocks s_Rocks[num_rocks];

void main(void)
{




//char *sc=(char *)malloc(4);

/*

// #ifndef SBSOUND
  ISRS *isrs = new ISRS();
  isrs->GetSBIRQ(sb16.BLASTER.SBIntr);
  sb16.GetISRPtr(isrs);
  sb16.SetModeBits(8);
  sb16.SetTransferMode(2);
  sb16.NewMixerSettings.vocleft=26;
  sb16.NewMixerSettings.vocright=26;
  sb16.NewMixerSettings.masterright=26;
  sb16.NewMixerSettings.masterleft=26;
  sb16.NewMixerSettings.cdleft=0;
  sb16.NewMixerSettings.cdright=0;
  sb16.SetMixerSettings();
  sb16.Load_Sounds();
  sb16.SoundRate=sb16.Sounds[0].SampleRate;
  sb16.ModeStereoMono=sb16.Sounds[0].Stereo;
  sb16.FillBuffer();
  isrs->SetupSBISR();
  sb16.SetupDMA();
  sb16.SetupDSP();
//  #endif
*/


  // make a second rockets for ship to
  my_rockets2=my_rockets;
  buffer=(unsigned char *)malloc(640*480);
  my_video.double_buffer=buffer;
  my_video.font_readtable();




// Setup video mode


// Init Rocket Data

Init_Rocks();


// Let's do Fire
my_fire.Load_Pcx("fire-01.pcx",6);
my_fire.Load_Pcx("fire-02.pcx",5);
my_fire.Load_Pcx("fire-03.pcx",4);
my_fire.Load_Pcx("fire-04.pcx",3);
my_fire.Load_Pcx("fire-05.pcx",2);
my_fire.Load_Pcx("fire-06.pcx",1);
my_fire.Load_Pcx("fire-07.pcx",0);


// Load Ship 2 PCX
my_spr2.Load_Pcx("ship2-01.pcx",0);
my_spr2.Load_Pcx("ship2-02.pcx",1);
my_spr2.Load_Pcx("ship2-03.pcx",2);

// Load Rock PCX
my_rocks.Load_Pcx("rock1.pcx",0);
my_rocks.Load_Pcx("rock2.pcx",1);

// Load Ship 1 PCX
my_spr.Load_Pcx("ship-01.pcx",0);
my_spr.Load_Pcx("ship-02.pcx",1);
my_spr.Load_Pcx("ship-03.pcx",2);

// Load Rocket PCX
my_rockets.Load_Pcx("m1.pcx",1);
my_rockets.Load_Pcx("m2.pcx",0);

my_pcx.Load_Pal("pal.pcx");
my_video.clear_buffer(buffer,0,640*480);

my_spr.x=256;
my_spr.y=416;

my_spr2.x=50;
my_spr2.y=416;

my_rockets.x=1;
my_rockets.y=1;

my_rockets.Draw(buffer);
my_spr.Draw(buffer);
my_spr2.Draw(buffer);

my_video.show_buffer(buffer);

my_spr2.trans=98;
my_rockets.trans=my_video.get_pixel(1,1);
my_spr.trans=98;
my_rocks.trans=98;
my_fire.trans=my_video.get_pixel(1,1);

my_spr.Erase(buffer);
my_rockets.Erase(buffer);

int r_startx=0,r_starty=0,r_count=0;



key_init();


while(!key_table[MAKE_ESC])
{

//start_timer=(int)(uclock());
// KeyBoard Controls

if (keys_active)
{


	if (key_table[MAKE_F])
	{
             if (!rocket_flag)
             {
                my_rockets.x=my_spr.x+20;
                my_rockets.y=my_spr.y+12;
                rocket_flag=true;
                my_rockets.cur_frame=0;
             }
          }


	if (key_table[MAKE_RIGHT])
	   if (my_spr.x	< 576)
           {
               if(my_spr.Hit(my_spr2,18))
	          my_spr.x=my_spr.x-4;
                  else
                  my_spr.x+=2;
           }

	if (key_table[MAKE_LEFT])
	   if (my_spr.x	> 1)
              if(my_spr.Hit(my_spr2,18))
                  my_spr.x=my_spr.x+4;
                  else
                  my_spr.x-=2;


	if (key_table[MAKE_UP])
	    if (my_spr.y > 1)
	       {
		 my_spr.y-=2;
		 shipflag=true;

               //  #ifndef SBSOUND
               //  if (SOUND_ON)
            //    sb16.Play(3);
               //  #endif
	       }

	if (key_table[MAKE_DOWN])
	    if (my_spr.y < 416)
		my_spr.y+=2;


// Ship2 controls for testing only
   	if (key_table[MAKE_R])
	{
             if (!rocket_flag2)
             {
                my_rockets2.x=my_spr2.x+15;
                my_rockets2.y=my_spr2.y+12;
                rocket_flag2=true;
                my_rockets2.cur_frame=0;
             }
          }



   if (key_table[MAKE_S])
   {
	    if (my_spr2.x <  576)
               if(my_spr2.Hit(my_spr,18))
                  my_spr2.x=my_spr2.x-4;
                    else
                  my_spr2.x+=2;

    }

    if (key_table[MAKE_A])
    {
	    if (my_spr2.x > 1)
               if(my_spr2.Hit(my_spr,18))
                  my_spr2.x=my_spr2.x+4;
               else
                  my_spr2.x-=2;
    }


    	if (key_table[MAKE_W])
	    if (my_spr2.y > 1)
	       {
		 my_spr2.y-=2;
		 shipflag2=true;
	       }

	if (key_table[MAKE_X])
	    if (my_spr2.y < 416)
		my_spr2.y+=2;
}// End of keyboard actions

   //my_video.font_putstring(300, 350,sc);


   Rocket_Animation();
   Rocket_Animation2();

   do_fire(); // Does fire
   Create_Rocks(); // Creates New Rocks when there destroyed
   Ship_Anim();  // Animation For ship1
   Ship_Anim2(); // Animation For ship2

   my_video.show_buffer(buffer);  // Updates the Screen with the buffer

   Destroy_Rocks(); // Kills Old rocks
   Erase_Rocket_Animation(); // erase's rockets
   Erase_Rocket_Animation2();
   Erase_Ship_Anim(); //  erase's
   Erase_Ship_Anim2(); // erase's ships moves
   my_video.clear_buffer(buffer,0,640*480);  // Clears entire screen
                                             // Cuz i'm lazy.
   } // End of while Loop


   // If Program exits  then calls these
   key_delete();
   free(buffer);
  // delete isrs;

}









// Function


void Create_Rocks(void)
{

 for (int a1=0;a1<num_rocks;a1++)
 {

  // Make a New Rock
  my_rocks.cur_frame=0;

 if (s_Rocks[a1].y >= 435 )
 {

     s_Rocks[a1].y=(rand() % 10)+45;
     s_Rocks[a1].type=(rand() %	1);
     s_Rocks[a1].speed=(rand() % 3)+1;
     s_Rocks[a1].x=(rand() % 596)+1;
 }

 my_rocks.x=s_Rocks[a1].x;
 my_rocks.y=s_Rocks[a1].y;

 // check Rocks a rock hit
 if ((rocket_flag == true) && (my_rockets.Hit(my_rocks,15) == true))
    {
  //    sb16.Play(2);
      rocket_flag=false;
      my_fire.x=my_rocks.x;
      my_fire.y=my_rocks.y;
      s_Rocks[a1].y=(rand() % 10)+45;
      s_Rocks[a1].x=(rand() % 596)+1;
      my_rocks.x=s_Rocks[a1].x;
      my_rocks.y=s_Rocks[a1].y;
      flagfire=true;

    }

    if ((rocket_flag2 == true) && (my_rockets2.Hit(my_rocks,15) == true))
    {
//       #ifndef SBSOUND
 //     if (SOUND_ON)
    //  sb16.Play(1);
   //   #endif

      rocket_flag2=false;
      my_fire.x=my_rocks.x;
      my_fire.y=my_rocks.y;
      s_Rocks[a1].y=(rand() % 10)+45;
      s_Rocks[a1].x=(rand() % 596)+1;
      my_rocks.x=s_Rocks[a1].x;
      my_rocks.y=s_Rocks[a1].y;
      flagfire=true;

    }


 if (my_spr.Hit(my_rocks,15))
 {


 //     sb16.Play(0);
      my_fire.x=my_spr.x-12;
      my_fire.y=my_spr.y-25;
      s_Rocks[a1].y=(rand() % 10)+45;
      s_Rocks[a1].x=(rand() % 596)+1;
      my_rocks.x=s_Rocks[a1].x;
      my_rocks.y=s_Rocks[a1].y;
      flagfire=true;

 }


  if (my_spr2.Hit(my_rocks,15))
 {


     // sb16.Play(0);
      my_fire.x=my_spr2.x;
      my_fire.y=my_spr2.y;
      s_Rocks[a1].y=(rand() % 10)+45;
      s_Rocks[a1].x=(rand() % 596)+1;
      my_rocks.x=s_Rocks[a1].x;
      my_rocks.y=s_Rocks[a1].y;
      flagfire=true;

 }


 my_rocks.Draw(buffer);

 }

}

void Init_Rocks(void)
{
  for (int a=0;a<num_rocks;a++)
   {
      s_Rocks[a].hit=false;
      s_Rocks[a].type=0; ///(rand() % 1);
      s_Rocks[a].speed=(rand() % 2)+1;
      s_Rocks[a].x=(rand() % 596)+1;
      s_Rocks[a].y=(rand() % 300)+45;
   }
}


void Destroy_Rocks(void)
{
  // Destroy rocks

 for (int a2=0;a2<num_rocks;a2++)
 {
     s_Rocks[a2].y+=s_Rocks[a2].speed;
     my_rocks.x=s_Rocks[a2].x;
     my_rocks.y=s_Rocks[a2].y;
     my_rockets.cur_frame=0;
     my_rocks.Erase(buffer);
 }

}

void Erase_Ship_Anim2(void)
{
   my_spr2.Erase(buffer);
}


void Erase_Ship_Anim(void)
{
   my_spr.Erase(buffer);

}

void Ship_Anim2(void)
{

if (shipflag2)
{


   if (my_spr2.cur_frame== 2)
   {
      shipflag2=false;
      my_spr2.cur_frame=2;
      my_spr2.Draw(buffer);
   }
   else
      my_spr2.cur_frame++;
}
   else
   my_spr2.cur_frame=0;

   my_spr2.Draw(buffer);

}



void Ship_Anim(void)
{

if (shipflag)
{


   if (my_spr.cur_frame	== 2)
   {
      shipflag=false;
      my_spr.cur_frame=2;
      my_spr.Draw(buffer);
   }
   else
      my_spr.cur_frame++;
}
else
   my_spr.cur_frame=0;

   my_spr.Draw(buffer);

}



void do_fire(void)
{

if (flagfire)
{

  my_fire.Draw(buffer);
  my_timer++;

  if (my_timer>15)
    {
     my_fire.cur_frame++;
     my_timer=0;
    }

  if (my_fire.cur_frame == 7)
    {
     my_fire.Erase(buffer);
     flagfire=false;
     my_fire.cur_frame=0;
    }


  }

}


void Rocket_Animation(void)
{

   if (rocket_flag)
   {
     my_rockets.y-=2;
     my_rockets.Draw(buffer);
   }



}

void Rocket_Animation2(void)
{

   if (rocket_flag2)
   {
     my_rockets2.y-=2;
     my_rockets2.Draw(buffer);
   }
}

void Erase_Rocket_Animation(void)
{
      if (rocket_flag)
      {

      if (my_rockets.y<26)
          rocket_flag=false;

       my_rockets.Erase(buffer);
       my_rockets.cur_frame=1;
      }



}
void Erase_Rocket_Animation2(void)
{
      if (rocket_flag2)
      {

      if (my_rockets2.y<26)
          rocket_flag2=false;

        my_rockets2.Erase(buffer);
        my_rockets2.cur_frame=1;
      }

}


