#include <iostream>
using namespace std;


#ifdef __APPLE__
#include <SDL/SDL.h>
#elif defined(linux)
#include <SDL/SDL.h>
#else
#include "SDL.h"
#endif




extern const int width, height;
extern const char* tytul;
extern const Uint8 defaultR;
extern SDL_Surface* screen;




void PixelTable();


void setPixel(int x, int y, Uint8 R, Uint8 G, Uint8 B);
SDL_Color getPixel(int x, int y);
Uint8* getPixelAddress(int x, int y);
inline bool czyWZakresie(int x, int y);
void loadBMP(char const* nazwa, int x, int y);
void clearScreen(Uint8 R, Uint8 G, Uint8 B);
void czyscEkran(Uint8 R, Uint8 G, Uint8 B);
int initSdl();
void czyscEkran(Uint8 R = 0, Uint8 G = 0, Uint8 B = 0);
void clearScreen(Uint8 R, Uint8 G, Uint8 B);
void loadBMP(char const* nazwa, int x, int y);
