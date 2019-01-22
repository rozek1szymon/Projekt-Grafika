#include "algorithms.h"
#include <algorithm>
#include <omp.h>

string MaxRange;
int iImgWidth, iImgHeight, rRange, gRange, bRange;
SDL_Color ** pixels = nullptr;
RLE * pixels1D = nullptr;
unsigned char * pxcolornumber = nullptr;
//vector <RLE> tab, vectorpixels, newPalette;
SDL_Surface* bmp;
bool greyflag;
//extern char * tabletofile;
//bool onlypalette;
const int DediCol = 128;

short maxrangemem[DediCol][DediCol]{};

vector<RLE> * testvector[DediCol];
RLE NewPalette[DediCol]{};

union Transfer2
{
	unsigned char a;
	unsigned short l : 8;
};




//funkcja tworz¹ca tablicê pixeli typu SDL_Color
//zapisuje równie¿ do tablicy jednowymiarowej typu RLE
void PixelTable()
{
	//niszczenie tablicy, chyba, ¿e nie istnieje
	if (pixels != nullptr)
	{
		for (int i = 0; i < iImgHeight; i++)
			delete[] pixels[i];
		delete pixels;
	}

	//tworzenie dynamicznej tablicy
	pixels = new SDL_Color *[iImgHeight];

	for (int i = 0; i < iImgHeight; i++)
		pixels[i] = new SDL_Color[iImgWidth];

	//zape³nianie tablicy
	for (int i = 0; i < iImgHeight; i++)
		for (int j = 0; j < iImgWidth; j++)
			pixels[i][j] = getPixel(j, i);

	f_save2Dto1D();
}

void f_save2Dto1D()
{
	//niszczenie tablicy, chyba, ¿e nie istnieje, czyszczenie wektorów
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


void greyscale()
{
	Uint8 grey;
	int scale;
	greyflag = 1;

	for(int i = 0; i < iImgHeight; i++)
		for (int j = 0; j < iImgWidth; j++)
		{
			scale =  (int)(0.299*(int)pixels[i][j].r + 0.587*(int)pixels[i][j].g + 0.114*(int)pixels[i][j].b );
			grey = (scale >> 1);
			setPixel(j, i, grey * 2, grey * 2, grey * 2);
		}
	SDL_Flip(screen);
}

