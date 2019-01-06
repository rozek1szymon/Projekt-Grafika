#ifdef __cplusplus
#include <cstdlib>
#else
#include <stdlib.h>

#endif
#ifdef _APPLE_
#include <SDL/SDL.h>
#else
//#include <SDL.h>
#endif
#include <math.h>
#define pi 3.14

extern const int width, height;
extern const char* tytul;
extern const Uint8 defaultR;
extern SDL_Surface* screen;

void setPixel(int x, int y, Uint8 R, Uint8 G, Uint8 B);
SDL_Color getPixel(int x, int y);
Uint8* getPixelAddress(int x, int y);
inline bool czyWZakresie(int x, int y);
void loadBMP(char const* nazwa, int x, int y);
void clearScreen(Uint8 R, Uint8 G, Uint8 B);
int initSdl();
void loadBMP(char const* nazwa, int x, int y);



extern int iImgWidth, iImgHeight, rRange, gRange, bRange;
extern SDL_Color ** pixels;
extern RLE * pixels1D;
//vector <RLE> tab, vectorpixels, newPalette;
extern SDL_Surface* bmp;
extern bool greyflag;
