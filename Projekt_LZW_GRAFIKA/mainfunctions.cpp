#include "mainfunctions.h"

int img_width, img_height;
SDL_Color ** pixels = nullptr;

//funkcja tworz�ca tablic� pixeli typu SDL_Color
void PixelTable()
{
	//tworzenie dynamicznej tablicy
	pixels = new SDL_Color * [img_height];

	for (int i = 0; i < img_height; i++)
		pixels[i] = new SDL_Color[img_width];

	//zape�nianie tablicy
	for (int i = 0; i < img_height; i++)
		for (int j = 0; j < img_width; j++)
			pixels[i][j] = getPixel(i, j);
}


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

void czyscEkran(Uint8 R, Uint8 G, Uint8 B)
{
	SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, R, G, B));
	SDL_Flip(screen);
}
void clearScreen(Uint8 R, Uint8 G, Uint8 B)
{
	SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, R, G, B));
	SDL_Flip(screen);
}

void loadBMP(char const* nazwa, int x, int y)
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

	//niszczenie tablicy, chyba, �e nie istnieje
	if (pixels != nullptr)
	{
		for (int i = 0; i < img_height; i++)
			delete [] pixels[i];
		delete pixels;
	}

	//zapis rozdzielczo�ci wy�wietlanego obrazka,
	//�eby wiedzie� jak du�a zrobi� tablice pixeli
	img_width = bmp->w;
	img_height = bmp->h;

	//tworzenie tablicy
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
		/* Pobieramy informacji ile bajt�w zajmuje jeden pixel */
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
