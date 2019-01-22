//
//  Converter.cpp
//  dupa
//
//  Created by Wojtek Filipowicz on 22/01/2019.
//  Copyright © 2019 Wojtek Filipowicz. All rights reserved.
//

#include "Converter.h"

Converter::Converter(SDL_Color** _pixels, int width, int height){
    pixels = _pixels;
    iImgWidth = width;
    iImgHeight = height;
    
    pixelsTo1D();
}

SDL_Color* Converter::fillColorPalette() {
    
    SDL_Color* ColorPalette = new SDL_Color(16);
    
    int i = 0;
    for (int R = 0; R <= 255; R += 85)
        for (int G = 0; G <= 255; G += 85)
            for (int B = 0; B <= 255; B += 85)
            {
                ColorPalette[i] = { R, G, B };
                ++i;
            }
    
    return ColorPalette;
}

SDL_Color* Converter::fillBWPalette() {
    
    SDL_Color* BWPalette = new SDL_Color(16);
    
    int i = 0;
    for (int j = 0; j < 255; j += 16)
    {
        BWPalette[i] = { j, j, j };
        ++i;
    }
    
    return BWPalette;
}

SDL_Color* Converter::MedianCutPalette() {
    
    if(!finish) {
        MedianCut(pixels1D, 0, ColorsNum - 1, iImgWidth*iImgHeight);
    } else {
        return newColors;
    }
}

int Converter::Maxrange(unsigned char r, unsigned char g, unsigned char b)
{
    int max = 0;
    if ((g >= r) && (g >= b)) max = 1;
    if ((b >= r) && (b >= g)) max = 2;
    return max;
}

void Converter::MedianCut(RLE * undertable, int left, int right, int size)
{
    if (right == left)
    {
        RLE avgColor = getAvgColor(undertable, size);
        newPalette.insert(newPalette.begin(), avgColor);
        NewPalette[left] = avgColor;
        
        for (int i = 0; i < size; i++)
        {
            setPixel(undertable[i].j , undertable[i].i, avgColor.r, avgColor.g, avgColor.b);
            
            SDL_Color newColor;
            newColor.r = avgColor.r;
            newColor.g = avgColor.g;
            newColor.b = avgColor.b;
            
            newColors[colorsCounter] = &newColor;
            colorsCounter++;
            
            pxcolornumber[undertable[i].i*iImgWidth + undertable[i].j] = left;
            
            if (colorsCounter == 16) {
                finish = true;
            }
        }
        SDL_Flip(screen);
        return;
    }
    
    // For finding color channel that has the most w ide range,
    // we need to keep their lower and upper bound.
    int iMaxR = (unsigned char)undertable[0].r, iMaxG = (unsigned char)undertable[0].g, iMaxB = (unsigned char)undertable[0].b,
    iMinR = (unsigned char)undertable[0].r, iMinG = (unsigned char)undertable[0].g, iMinB = (unsigned char)undertable[0].b;
    int maxRange;
    
    for (int j = 0; j < size; j++)
    {
        if (iMaxR < (unsigned char)undertable[j].r) iMaxR = (unsigned char)undertable[j].r;
        if (iMaxG < (unsigned char)undertable[j].g) iMaxG = (unsigned char)undertable[j].g;
        if (iMaxB < (unsigned char)undertable[j].b) iMaxB = (unsigned char)undertable[j].b;
    }
    for (int j = 0; j < size; j++)
    {
        if (iMinR >(unsigned char)undertable[j].r) iMinR = (unsigned char)undertable[j].r;
        if (iMinG >(unsigned char)undertable[j].g) iMinG = (unsigned char)undertable[j].g;
        if (iMinB >(unsigned char)undertable[j].b) iMinB = (unsigned char)undertable[j].b;
    }
    
    rRange = iMaxR - iMinR;
    gRange = iMaxG - iMinG;
    bRange = iMaxB - iMinB;
    
    maxrangemem[left][right] = maxRange = Maxrange(rRange, gRange, bRange);
    
    Heap kopiec(pixels1D, size);
    
    if (maxRange == 0)
        kopiec.Sortby(sortr);
    else if (maxRange == 1)
        kopiec.Sortby(sortg);
    else
        kopiec.Sortby(sortb);
    
    kopiec.RestoreAtt();
    kopiec.HeapSort();
    
    if (size % 2 == 0)
    {
        //upper
        MedianCut(undertable + size / 2, (left + right) / 2 + 1, right, size / 2);
        //lower
        MedianCut(undertable, left, (left + right) / 2, size / 2);
    }
    else
    {
        //upper
        MedianCut(undertable + size / 2+1, (left + right) / 2+1, right, size / 2);
        //lower
        MedianCut(undertable, left, (left + right) / 2 , size / 2+1);
    }
}

RLE getAvgColor(std::vector<RLE>& pixels)
{
    /// Funkcja zwraca úredniπ z pikseli znajdujπcych siÍ w vectorze pixels
    int red = 0, green = 0, blue = 0;
    
    for (unsigned int i = 0; i < pixels.size(); i++)
    {
        red +=   (int)pixels[i].r;
        green += (int)pixels[i].g;
        blue +=  (int)pixels[i].b;
    }
    
    RLE color;
    color.r = (uint8_t) (red / pixels.size());
    color.g = (uint8_t) (green / pixels.size());
    color.b = (uint8_t) (blue / pixels.size());
    
    return color;
}

void Converter::pixelsTo1D()
{
    //niszczenie tablicy, chyba, øe nie istnieje, czyszczenie wektorÛw
    if (pixels1D != nullptr)
        delete[] pixels1D;
    tab.clear();
    vectorpixels.clear();
    
    pixels1D = new RLE[iImgHeight*iImgWidth];
    int k = 0;
    for (int i = 0; i<iImgHeight; i++)
        for (int j = 0; j < iImgWidth; j++)
        {
            pixels1D[k].r = pixels[i][j].r;
            pixels1D[k].g = pixels[i][j].g;
            pixels1D[k].b = pixels[i][j].b;
            pixels1D[k].i = i;
            pixels1D[k].j = j;
            k++;
        }
}
