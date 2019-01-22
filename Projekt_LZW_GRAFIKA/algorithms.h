#pragma once
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <math.h>
#include <algorithm>
#include <windows.h>
#include <iomanip>

#include <iostream>
#include <vector>
#include "SDL_Functions.h"
using namespace std;

#ifdef __APPLE__
#include <SDL/SDL.h>
#elif defined(linux)
#include <SDL/SDL.h>
#else
//#include <SDL.h>
#endif

#undef main

extern const int width, height;
extern const char* tytul;
extern const Uint8 defaultR;
extern SDL_Surface* screen;


void PixelTable();

bool operator == (RLE &a, RLE &b);
bool operator != (RLE &a, RLE &b);
//void HowManyColorsRLE(RLE * pxrle, int size);
RLE getAvgColor(vector<RLE>& pixels);
void showPalette();
//////////
void greyscale();
void f_save2Dto1D();
