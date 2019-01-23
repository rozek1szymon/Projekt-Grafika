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
#include <math.h>

#include "Converter.h"

using namespace std;


void PixelTable();
SDL_Surface *screen;
int width = 900;
int height = 600;
char const* tytul = "GKiM - Lab 2 - Nazwisko Imie";

void DitheringKolor();
void DitheringSzarosc();
void setPixel(int x, int y, Uint8 R, Uint8 G, Uint8 B);
SDL_Color getPixel (int x, int y);
void czyscEkran(Uint8 R, Uint8 G, Uint8 B);

int img_width, img_height;
SDL_Color ** pixels = NULL;

static SDL_Color* newColors = new SDL_Color[16];

Converter* converter;

SDL_Color WhichColorFitTheMost(SDL_Color kolor1[16], SDL_Color kolor2)
{
    double TheClosest=255;
    SDL_Color theclosestcolor=kolor1[0];

     for(int i=0; i<16; i++)
     {
        double pierwiastek = sqrt((kolor2.r-kolor1[i].r)*(kolor2.r-kolor1[i].r) + (kolor2.g-kolor1[i].g)*(kolor2.g-kolor1[i].g)+(kolor2.b-kolor1[i].b)*(kolor2.b-kolor1[i].b));
        if(pierwiastek<TheClosest)
        {
            theclosestcolor=kolor1[i];
            TheClosest=pierwiastek;

        }

     }
return theclosestcolor;
}

void Funkcja1()
{
    SDL_Color* kolor1 = converter->fillColorPalette();
    SDL_Color* kolor2 = converter->fillBWPalette();
    SDL_Color* kolor3 = converter->MedianCutPalette();


    SDL_Color kolor;
    for(int xx=0; xx<width/2; xx++)
    {
        for(int yy=0; yy<height/2; yy++)
        {
            kolor=getPixel(xx,yy);

             //DitheringKolor();
            setPixel(xx,yy,WhichColorFitTheMost(kolor1,kolor).r,WhichColorFitTheMost(kolor1,kolor).g,WhichColorFitTheMost(kolor1,kolor).b);
           // setPixel(xx,yy,WhichColorFitTheMost(kolor2,kolor).r,WhichColorFitTheMost(kolor2,kolor).g,WhichColorFitTheMost(kolor2,kolor).b);
           // setPixel(xx,yy,WhichColorFitTheMost(kolor3,kolor).r,WhichColorFitTheMost(kolor3,kolor).g,WhichColorFitTheMost(kolor3,kolor).b);
          // DitheringSzarosc();
        }
    }
    for(int xx=0; xx<width/2; xx++)
    {
        for(int yy=0; yy<height/2; yy++)
        {
            kolor=getPixel(xx,yy);
           DitheringSzarosc();
           //DitheringKolor();
        }
    }
    SDL_Flip(screen);
}
//funkcja tworzπca tablicÍ pixeli typu SDL_Color
void PixelTable()
{
    //tworzenie dynamicznej tablicy
    pixels = new SDL_Color * [img_height];

    for (int i = 0; i < img_height; i++)
        pixels[i] = new SDL_Color[img_width];

    //zape≥nianie tablicy
    for (int i = 0; i < img_height; i++)
    {
       for (int j = 0; j < img_width; j++)
        {
            pixels[i][j] = getPixel(i, j);

        }
    }
}

SDL_Color* dedicatedColors = new SDL_Color[16];

void Funkcja2 () {
    converter->MedianCutPalette();
}

void DitheringSzarosc()
{
    int BW;
    SDL_Color kolor;
    float bledy[(width/2) + 2][(height/2) + 2];
    memset(bledy, 0, sizeof(bledy));
    int blad = 0;
    int przesuniecie = 1; // aby nie wyjsc ponizej (-1) tabeli bledow


    for(int y = 0; y < (height / 2); y++)
        for(int x = 0; x < (width / 2); x++)
        {
            kolor = getPixel(x, y);
            BW = kolor.r * 0.299 + kolor.g * 0.587 + kolor.b * 0.114;
            //setPixel(x, y + (height/2), BW, BW, BW);

            BW += bledy[x + przesuniecie][y];
            if(BW > 127)
            {
                setPixel(x + width / 2, y + height / 2, 255, 255, 255);
                blad = BW - 255;
            }
            else
            {
                setPixel(x + width / 2, y + height / 2, 0, 0, 0);
                blad = BW;
            }

            bledy[x + przesuniecie + 1][y] += (blad * 7.0/16.0);
            bledy[x + przesuniecie - 1][y + 1] += (blad * 3.0/16.0);
            bledy[x + przesuniecie + 1][y + 1] += (blad * 5.0/16.0);
            bledy[x + przesuniecie + 1][y + 1] += (blad * 1.0/16.0);

        }
         SDL_Flip(screen);

}

void DitheringKolor()
{
    SDL_Color kolor, kolor2;
    for(int y = 0; y < (height / 2) -1; y++)
        for(int x = 1; x < (width / 2) ; x++)
        {
            kolor = getPixel(x, y);

            float oldR = kolor.r;
            float oldG = kolor.g;
            float oldB = kolor.b;

            int factor = 15; // mamy do dyspozycji 10 roznych wartosci dla g a tylko 2 dla r i b
            int newR = round(oldR*factor / 255) * (255 / factor);
            int newG = round(factor * oldG / 255) * (255 / factor);
            int newB = round(oldB*factor/ 255) * (255 / factor);
            setPixel(x + width / 2, y + height / 2, newR, newG, newB);

            float errR = oldR - newR;
            float errG = oldG - newG;
            float errB = oldB - newB;


            kolor2 = getPixel(x+1,y);
            float r = kolor2.r;
            float g = kolor2.g;
            float b = kolor2.b;
            r = r + (errR * 7.0/16.0);
            g = g + (errG * 7.0/16.0);
            b = b + (errB * 7.0/16.0);
            setPixel(x + width / 2 + 1, y + height / 2, r, g, b);

            kolor2 = getPixel(x-1,y+1);
            r = kolor2.r;
            g = kolor2.g;
            b = kolor2.b;
            r = r + (errR * 3.0/16.0);
            g = g + (errG * 3.0/16.0);
            b = b + (errB * 3.0/16.0);
            setPixel(x + width / 2 - 1, y + height / 2 + 1, r, g, b);

            kolor2 = getPixel(x ,y+1);
            r = kolor2.r;
            g = kolor2.g;
            b = kolor2.b;
            r = r + (errR * 5.0/16.0);
            g = g + (errG * 5.0/16.0);
            b = b + (errB * 5.0/16.0);
            setPixel(x + width / 2, y + height / 2 + 1, r, g, b);

            kolor2 = getPixel(x+1,y+1);
            r = kolor2.r;
            g = kolor2.g;
            b = kolor2.b;
            r = r + errR * 1.0/16.0;
            g = g + errG * 1.0/16.0;
            b = b + errB * 1.0/16.0;
            setPixel(x + width / 2 + 1, y + height / 2 + 1, r, g, b);
        }
         SDL_Flip(screen);
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
    if (pixels !=NULL)
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
    converter = new Converter(pixels, img_width, img_height, screen);
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
                    if (event.key.keysym.sym == SDLK_2)
                        Funkcja2();

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

