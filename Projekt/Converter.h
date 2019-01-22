//
//  Converter.hpp
//  dupa
//
//  Created by Wojtek Filipowicz on 22/01/2019.
//  Copyright © 2019 Wojtek Filipowicz. All rights reserved.
//

#ifndef Converter_h
#define Converter_h

#include <stdio.h>
#include <vector>

#include "Heap.h"

struct SDL_Color {
    int r, g, b;
};

const int ColorsNum = 16;

class Converter {
private:
    Converter(SDL_Color** _pixels, int width, int height);
    
    int iImgWidth, iImgHeight, rRange, gRange, bRange;
    vector <RLE> tab, vectorpixels, newPalette;
    SDL_Color ** pixels;
    RLE * pixels1D;
    short maxrangemem[ColorsNum][ColorsNum]{};
    unsigned char * pxcolornumber = nullptr;
    vector<RLE> * testvector[ColorsNum];
    RLE NewPalette[ColorsNum]{};
    
    void MedianCut(RLE * undertable, int left, int right, int size);
    RLE getAvgColor(vector<RLE>& pixels);
    int Maxrange(unsigned char r, unsigned char g, unsigned char b);
    void pixelsTo1D();
    
    int colorsCounter = 0;
    SDL_Color* newColors = new SLD_Colors(16);
    bool finish = false;
   
public:
    SDL_Color* fillColorPalette();
    SDL_Color* fillBWPalette();
    SDL_Color* MedianCutPalette();
};

#endif /* Converter_hpp */
