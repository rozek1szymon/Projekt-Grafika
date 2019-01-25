#include "Converter.h"

Converter::Converter(SDL_Color** _pixels, int width, int height, SDL_Surface *screen){
    pixels = _pixels;
    ImgWidth = width;
    ImgHeight = height;
    newColors = new SDL_Color[16];

    pixelsTo1D();
}

SDL_Color* Converter::fillColorPalette() {

    SDL_Color* ColorPalette = new SDL_Color[16];

    ColorPalette[0]= {0,0,0};
    ColorPalette[1]= {0,0,255};
    ColorPalette[2]= {0,107,0};
    ColorPalette[3]= {0,97,255};
    ColorPalette[4]= {0,190,0};
    ColorPalette[5]= {0,185,255};
    ColorPalette[6]= {0,255,0};
    ColorPalette[7]= {0,255,255};
    ColorPalette[8]= {255,0,0};
    ColorPalette[9]= {255,0,255};
    ColorPalette[10]= {255,93,0};
    ColorPalette[11]= {255,82,255};
    ColorPalette[12]= {255,183,0};
    ColorPalette[13]= {255,179,255};
    ColorPalette[14]= {255,255,0};
    ColorPalette[15]= {255,255,255};

    return ColorPalette;
}

SDL_Color* Converter::fillBWPalette() {

    SDL_Color* BWPalette = new SDL_Color[16];

    int i = 0;
    for (Uint8 j = 0; j < 255; j += 15)
    {
        BWPalette[i] = { j, j, j };
        ++i;
    }

    return BWPalette;
}

SDL_Color* Converter::MedianCutPalette() {
    preMedianCut();
    while (!finish) {
        //wait for finish
    }
    return newColors;
}

int Converter::Maxrange(unsigned char r, unsigned char g, unsigned char b)
{
    int max = 0;
    if ((g >= r) && (g >= b)) max = 1;
    if ((b >= r) && (b >= g)) max = 2;
    return max;
}

 void Converter::preMedianCut()
{
	if (pxcolornumber != nullptr) {
        delete[] pxcolornumber;
	}
    pxcolornumber = new unsigned char[ImgWidth*ImgHeight];

	MedianCut(pixels1D, 0, ColorsNum - 1, ImgWidth*ImgHeight);
}

void Converter::MedianCut(RLE* undertable, int left, int right, int size)
{
    if (right == left)
    {
        RLE avgColor = getAvgColor(undertable, size);
        newPalette.insert(newPalette.begin(), avgColor);
        NewPalette[left] = avgColor;

        for (int i = 0; i < size; i++)
        {
            pxcolornumber[undertable[i].i*ImgWidth + undertable[i].j] = left;
        }

        if (newPalette.size()==16) {
            for (int i = 0; i < 16; i++) {
                SDL_Color kolor = {NewPalette[i].r, NewPalette[i].g, NewPalette[i].b};
                newColors[i] = kolor;
            }
            finish = true;
        }

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

RLE Converter::getAvgColor(RLE* pixels, int size)
{
    /// Funkcja zwraca úredniπ z pikseli znajdujπcych siÍ w vectorze pixels
    int red = 0, green = 0, blue = 0;

    for (int i = 0; i < size; i++)
    {
        red +=   (int)pixels[i].r;
        green += (int)pixels[i].g;
        blue +=  (int)pixels[i].b;
    }

    RLE color;
    color.r = red / size;
    color.g = green / size;
    color.b = blue / size;

    return color;
}

void Converter::pixelsTo1D()
{
    //niszczenie tablicy, chyba, øe nie istnieje, czyszczenie wektorÛw
    if (pixels1D != NULL)
        delete[] pixels1D;
    tab.clear();
    vectorpixels.clear();

    pixels1D = new RLE[ImgHeight*ImgWidth];
    int k = 0;
    for (int i = 0; i<ImgWidth; i++)
        for (int j = 0; j < ImgHeight; j++)
        {
            pixels1D[k].r = pixels[i][j].r;
            pixels1D[k].g = pixels[i][j].g;
            pixels1D[k].b = pixels[i][j].b;
            pixels1D[k].i = i;
            pixels1D[k].j = j;
            k++;
        }
}

SDL_Color** Converter::Dithering(SDL_Color** pixels) {

    SDL_Color kolor, kolor2;
    for(int x = 1; x < ImgWidth - 1; x++)
        for(int y = 0; y < ImgHeight; y++)
        {
            kolor = pixels[x][y];
            float oldR = kolor.r;
            float oldG = kolor.g;
            float oldB = kolor.b;

            int factor = 15; // uzywamy 2 wartosci
            Uint8 newR = round(factor * oldR / 255) * (255 / factor);
            Uint8 newG = round(factor * oldG / 255) * (255 / factor);
            Uint8 newB = round(factor * oldB / 255) * (255 / factor);
            pixels[x][y] = {newR, newG, newB};

            float errR = oldR - newR;
            float errG = oldG - newG;
            float errB = oldB - newB;

            kolor2 = pixels[x+1][y];
            float r = kolor2.r;
            float g = kolor2.g;
            float b = kolor2.b;
            r = r + (errR * 7.0/16.0);
            g = g + (errG * 7.0/16.0);
            b = b + (errB * 7.0/16.0);
            pixels[x+1][y] = {r, g, b};

            kolor2 = pixels[x-1][y+1];
            r = kolor2.r;
            g = kolor2.g;
            b = kolor2.b;
            r = r + (errR * 3.0/16.0);
            g = g + (errG * 3.0/16.0);
            b = b + (errB * 3.0/16.0);
            pixels[x-1][y+1] = {r, g, b};

            kolor2 = pixels[x][y+1];
            r = kolor2.r;
            g = kolor2.g;
            b = kolor2.b;
            r = r + (errR * 5.0/16.0);
            g = g + (errG * 5.0/16.0);
            b = b + (errB * 5.0/16.0);
            pixels[x][y+1] = {r, g, b};

            kolor2 = pixels[x+1][y+1];;
            r = kolor2.r;
            g = kolor2.g;
            b = kolor2.b;
            r = r + errR * 1.0/16.0;
            g = g + errG * 1.0/16.0;
            b = b + errB * 1.0/16.0;
            pixels[x+1][y+1] = {r, g, b};
        }

        return pixels;
}
