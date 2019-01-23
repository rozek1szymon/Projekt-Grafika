//
//  Converter.hpp
//  dupa
//
//  Created by Wojtek Filipowicz on 22/01/2019.
//  Copyright © 2019 Wojtek Filipowicz. All rights reserved.
//

#ifndef Converter_h
#define Converter_h

#ifdef __APPLE__
#include <SDL/SDL.h>
#else
#include <SDL.h>
#endif
#include <stdio.h>
#include <vector>

#include "Heap.h"

using namespace std;

const int ColorsNum = 16;

class Converter {
private:

    int iImgWidth, iImgHeight, rRange, gRange, bRange;
    vector <RLE> tab, vectorpixels, newPalette;
    SDL_Color ** pixels;
    RLE * pixels1D;
    short maxrangemem[ColorsNum][ColorsNum]{};
    unsigned char * pxcolornumber = NULL;
    vector<RLE> * testvector[ColorsNum];
    RLE NewPalette[ColorsNum]{};

    void preMedianCut();
    void MedianCut(RLE * undertable, int left, int right, int size);
    RLE getAvgColor(RLE* pixels, int size);
    int Maxrange(unsigned char r, unsigned char g, unsigned char b);
    void pixelsTo1D();

    SDL_Surface *screen0;
    void setPixel(int x, int y, Uint8 R, Uint8 G, Uint8 B);

    SDL_Color* newColors;
    bool finish = false;

public:
    Converter(SDL_Color** _pixels, int width, int height, SDL_Surface *screen);

    SDL_Color* fillColorPalette();
    SDL_Color* fillBWPalette();
    SDL_Color* MedianCutPalette();
};

#endif /* Converter_hpp */
