#include "SDL_Functions.h"



int initSdl()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("Unable to init SDL: %s\n", SDL_GetError());
		return 1;
	}

	atexit(SDL_Quit);

	const int bitDepth = 32;

	/// create a new window
	screen = SDL_SetVideoMode(width, height, bitDepth, SDL_HWSURFACE | SDL_DOUBLEBUF);
	if (!screen)
	{
		printf("Unable to set video: %s\n", SDL_GetError());
		return 1;
	}

	SDL_WM_SetCaption(tytul, NULL);

	return 0;
}

void clearScreen(Uint8 R, Uint8 G, Uint8 B)
{
	SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, R, G, B));
	SDL_Flip(screen);
}

void loadBMP(char const* nazwa, int x, int y)
{
	bmp = SDL_LoadBMP(nazwa);
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

	//zapis rozdzielczoœci wyœwietlanego obrazka,
	//¿eby wiedzieæ jak du¿a zrobiæ tablice pixeli
	iImgWidth = bmp->w;
	iImgHeight = bmp->h;
	greyflag = 0;

	//tworzenie tablicy 2D i 1D
	PixelTable();
}

inline bool czyWZakresie(int x, int y)
{
	return x >= 0 && x < screen->w && y >= 0 && y < screen->h;
}

Uint8* getPixelAddress(int x, int y)
{
	if (czyWZakresie(x, y))
	{
		/* Pobieramy informacji ile bajtów zajmuje jeden pixel */
		const int bpp = screen->format->BytesPerPixel;

		/* Obliczamy adres pixela */
		return (Uint8*)screen->pixels + y * screen->pitch + x * bpp;
	}
	return NULL;
}

SDL_Color getPixel(int x, int y)
{
	Uint8* pixelAddress = getPixelAddress(x, y);

	SDL_Color color = {};
	if (pixelAddress)
	{
		Uint32 col = 0;

		memcpy(&col, pixelAddress, screen->format->BytesPerPixel);
		SDL_GetRGB(col, screen->format, &color.r, &color.g, &color.b);
	}

	return color;
}

void setPixel(int x, int y, Uint8 R, Uint8 G, Uint8 B)
{
	Uint8* pixelAddress = getPixelAddress(x, y);
	if (pixelAddress)
	{
		Uint32 pixel = SDL_MapRGB(screen->format, R, G, B);

		switch (screen->format->BytesPerPixel)
		{
		case 1: //8-bit
			*pixelAddress = pixel;
			break;

		case 2: //16-bit
			*(Uint16*)pixelAddress = pixel;
			break;

		case 3: //24-bit
			if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
				pixelAddress[0] = (pixel >> 16) & 0xff;
				pixelAddress[1] = (pixel >> 8) & 0xff;
				pixelAddress[2] = pixel & 0xff;
			}
			else {
				pixelAddress[0] = pixel & 0xff;
				pixelAddress[1] = (pixel >> 8) & 0xff;
				pixelAddress[2] = (pixel >> 16) & 0xff;
			}
			break;
		case 4: //32-bit
			*(Uint32*)pixelAddress = pixel;
			break;
		}
	}
}
