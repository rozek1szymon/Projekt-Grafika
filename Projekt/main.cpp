#ifdef __cplusplus
#include <cstdlib>
#else
#include <stdlib.h>

#endif
#ifdef __APPLE__
#include <SDL/SDL.h>
#else
#include <SDL.h>
#endif
#include <math.h>
#define pi 3.14
#include <time.h>
#include <iostream>
#include <map>
#include <vector>
using namespace std;



SDL_Surface *screen;
int width = 900;
int height = 600;
char const* tytul = "GKiM - Lab 2 - Nazwisko Imie";


void setPixel(int x, int y, Uint8 R, Uint8 G, Uint8 B);
SDL_Color getPixel (int x, int y);
void czyscEkran(Uint8 R, Uint8 G, Uint8 B);

int img_width, img_height;
SDL_Color ** pixels = nullptr;

//funkcja tworzπca tablicÍ pixeli typu SDL_Color
void PixelTable()
{
    //tworzenie dynamicznej tablicy
    pixels = new SDL_Color * [img_height];
    
    for (int i = 0; i < img_height; i++)
        pixels[i] = new SDL_Color[img_width];
    
    //zape≥nianie tablicy
    for (int i = 0; i < img_height; i++)
        for (int j = 0; j < img_width; j++)
            pixels[i][j] = getPixel(i, j);
}

void setPixel(int x, int y, Uint8 R, Uint8 G, Uint8 B)
{
    if ((x>=0) && (x<width) && (y>=0) && (y<height))
    {
        /* Zamieniamy poszczególne składowe koloru na format koloru pixela */
        Uint32 pixel = SDL_MapRGB(screen->format, R, G, B);
        
        /* Pobieramy informacji ile bajtów zajmuje jeden pixel */
        int bpp = screen->format->BytesPerPixel;
        
        /* Obliczamy adres pixela */
        Uint8 *p = (Uint8 *)screen->pixels + y * screen->pitch + x * bpp;
        
        /* Ustawiamy wartość pixela, w zależności od formatu powierzchni*/
        switch(bpp)
        {
            case 1: //8-bit
                *p = pixel;
                break;
                
            case 2: //16-bit
                *(Uint16 *)p = pixel;
                break;
                
            case 3: //24-bit
                if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
                    p[0] = (pixel >> 16) & 0xff;
                    p[1] = (pixel >> 8) & 0xff;
                    p[2] = pixel & 0xff;
                } else {
                    p[0] = pixel & 0xff;
                    p[1] = (pixel >> 8) & 0xff;
                    p[2] = (pixel >> 16) & 0xff;
                }
                break;
                
            case 4: //32-bit
                *(Uint32 *)p = pixel;
                break;
                
        }
        /* update the screen (aka double buffering) */
    }
}


void ladujBMP(char const* nazwa, int x, int y)
{
    SDL_Surface* bmp = SDL_LoadBMP(nazwa);
    if (!bmp)
    {
        printf("Unable to load bitmap: %s\n", SDL_GetError());
    }
    else
    {
        SDL_Rect dstrect;
        dstrect.x = x;
        dstrect.y = y;
        SDL_BlitSurface(bmp, 0, screen, &dstrect);
        SDL_Flip(screen);
        SDL_FreeSurface(bmp);
    }
    
    //niszczenie tablicy, chyba, øe nie istnieje
    if (pixels != nullptr)
    {
        for (int i = 0; i < img_height; i++)
            delete [] pixels[i];
        delete pixels;
    }
    
    //zapis rozdzielczoúci wyúwietlanego obrazka,
    //øeby wiedzieÊ jak duøa zrobiÊ tablice pixeli
    img_width = bmp->w;
    img_height = bmp->h;
    
    //tworzenie tablicy
    PixelTable();
}


void czyscEkran(Uint8 R, Uint8 G, Uint8 B)
{
    SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, R, G, B));
    SDL_Flip(screen);
}

SDL_Color getPixel (int x, int y) {
    SDL_Color color ;
    Uint32 col = 0 ;
    if ((x>=0) && (x<width) && (y>=0) && (y<height)) {
        //determine position
        char* pPosition=(char*)screen->pixels ;
        //offset by y
        pPosition+=(screen->pitch*y) ;
        //offset by x
        pPosition+=(screen->format->BytesPerPixel*x);
        //copy pixel data
        memcpy(&col, pPosition, screen->format->BytesPerPixel);
        //convert color
        SDL_GetRGB(col, screen->format, &color.r, &color.g, &color.b);
    }
    return ( color ) ;
}


int main ( int argc, char** argv )
{
    // console output
    freopen( "CON", "wt", stdout );
    freopen( "CON", "wt", stderr );
    
    // initialize SDL video
    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "Unable to init SDL: %s\n", SDL_GetError() );
        return 1;
    }
    
    // make sure SDL cleans up before exit
    atexit(SDL_Quit);
    
    // create a new window
    screen = SDL_SetVideoMode(width, height, 32,
                              SDL_HWSURFACE|SDL_DOUBLEBUF);
    if ( !screen )
    {
        printf("Unable to set video: %s\n", SDL_GetError());
        return 1;
    }
    
    SDL_WM_SetCaption( tytul , NULL );
    // program main loop
    bool done = false;
    while (!done)
    {
        // message processing loop
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            // check for messages
            switch (event.type)
            {
                    // exit if the window is closed
                case SDL_QUIT:
                    done = true;
                    break;
                    
                    // check for keypresses
                case SDL_KEYDOWN:
                {
                    // exit if ESCAPE is pressed
                    if (event.key.keysym.sym == SDLK_ESCAPE)
                        done = true;
                    if (event.key.keysym.sym == SDLK_1)
                        Funkcja1();
                    
                    if (event.key.keysym.sym == SDLK_a)
                        ladujBMP("obrazek1.bmp", 0, 0);
                    if (event.key.keysym.sym == SDLK_s)
                        ladujBMP("obrazek2.bmp", 0, 0);
                    if (event.key.keysym.sym == SDLK_d)
                        ladujBMP("obrazek3.bmp", 0, 0);
                    if (event.key.keysym.sym == SDLK_f)
                        ladujBMP("obrazek4.bmp", 0, 0);
                    if (event.key.keysym.sym == SDLK_g)
                        ladujBMP("obrazek5.bmp", 0, 0);
                    if (event.key.keysym.sym == SDLK_b)
                        czyscEkran(0, 0, 10);          break;
                }
            } // end switch
        } // end of message processing
        
    } // end main loop
    
    
    // all is well ;)
    printf("Exited cleanly\n");
    return 0;
}

