
// Sprite lib for project code named Dextor
// Written by Kevin Fowlks
// Lead programmer / Sound SFX / Design
// Created March 19,1998


class Sprite:public Pcx_Loader{

public:
        int t;
        //int trans;
        int width,height;
        int spr_width,spr_height;
        int x,y;
        int num_frames;
        int num_waves;
        int cur_frame;
        unsigned char trans;
        unsigned char *buffer[10];
        unsigned char *background_buffer;
        int  *snd_buffer[10];
        int health;
        int face_direction;
        Sprite():spr_width(0),spr_height(0){};
        Sprite(int spr_h,int spr_w,int n_frames);
        void Blit(unsigned char *dest);
        void Get_BackGrd(unsigned char *dest);
        void Blit_Trans(unsigned char *dest);
        virtual bool Load_Pcx(char *,int);
        void Draw(unsigned char *dest);
        void Erase(unsigned char *dest);
        bool Hit(Sprite temp,int degree);
        ~Sprite();
        };

 




