#include "filefunctions.h"
#include "SDL_Functions.h"
//docelowa tablica kolorów z polami bitowymi

vector <SDL_Color> tempnewpalette;

unsigned char * outchar = nullptr;

struct Colors
{
	unsigned char r : 2;
	unsigned char g : 3;
	unsigned char b : 2;
};

struct ColorsShort
{
	unsigned short r : 2;
	unsigned short g : 3;
	unsigned short b : 2;
};

extern SDL_Color ** pixels;
extern int iImgWidth, iImgHeight;
extern SDL_Surface* screen;
extern bool greyflag;
unsigned char * tabletofile = nullptr;
char * predtable = nullptr;

extern unsigned char * pxcolornumber;

union Transfer
{
	unsigned char a;
	Colors s;
	unsigned short l : 8;
	ColorsShort c;
};



struct FileHeader
{
	const char Format[4] = "EXP";
	int Filesize;
	int Colormode;
	FileHeader()
	{
		Filesize = 100;
	}
};

struct ImageHeader
{
	int HeaderSize;
	int Width;
	int Height;
	int ImageSize;
	SDL_Color Colors[128]{};
	ImageHeader()
	{
		HeaderSize = 528;
		Width = iImgWidth;
		Height = iImgHeight;
		ImageSize = iImgWidth*iImgHeight;
	}
};

void SaveBmp()
{
	SDL_Surface *bmp = SDL_CreateRGBSurface(0, iImgWidth, iImgHeight, 32, 0, 0, 0, 0);
	SDL_BlitSurface(screen, 0, bmp, 0);
	string path = GetFolderName();
	cout << "Jak ma sie nazywac plik?" << endl;
	string name;
	cin >> name;
	path.append("\\");
	path.append(name);
	path.append(".bmp");
	SDL_SaveBMP(bmp, path.c_str());
	cout << "Zapisano plik: " << path << endl;
}

void SaveExp()
{
	if (pxcolornumber == nullptr)
	{
		cout << "Nie mozna stworzyc pliku, poniewaz nie wybrano ani narzuconej, ani dedykowanej palety." << endl;
		cout << "Wcisnij 3 lub 4 przed zapisem do .exp" << endl;
		return;
	}

	FileHeader fheader;
	ImageHeader iheader;
	fstream obrazek;
	string path;
	cout << "Gdzie chcesz zapisac plik?" << endl;
	path = GetFolderName();
	cout << "Jak ma sie nazywac plik?" << endl;
	string name;
	cin >> name;
	path.append("\\");
	path.append(name);
	path.append(".exp");
	obrazek.open(path, ios::out | ios::binary);

	if (greyflag)
		fheader.Colormode = 0;
	else
		fheader.Colormode = 1;
	int compsize = Compression2(pxcolornumber, iImgWidth*iImgHeight);
	fheader.Filesize = 12 + 528 + compsize;

	for (int i = 0; i < 128; i++)
	{
		iheader.Colors[i].r = NewPalette[i].r;
		iheader.Colors[i].g = NewPalette[i].g;
		iheader.Colors[i].b = NewPalette[i].b;
	}

	obrazek.write((char *)&fheader, sizeof(FileHeader));
	obrazek.write((char *)&iheader, sizeof(iheader));
	obrazek.write((char *)outchar, compsize);
	obrazek.close();

	cout << "Zapisano: " << path << endl;
}

void LoadExp(string path)
{
	FileHeader fheader;
	ImageHeader iheader;
	fstream obrazek;
	int c = 0;
	obrazek.open(path, ios::in | ios::binary);

	for (int i = 0; i < iImgHeight; i++)
		delete[] pixels[i];

	delete[] pixels;
	pixels = nullptr;

	//loading headers
	obrazek.read((char *)&fheader, sizeof(FileHeader));
	obrazek.read((char *)&iheader, sizeof(ImageHeader));

	iImgWidth = iheader.Width;
	iImgHeight = iheader.Height;

	//table for reading pixels

	for (int i = 0; i < 128; i++)
	{
		NewPalette[i].r = iheader.Colors[i].r;
		NewPalette[i].g = iheader.Colors[i].g;
		NewPalette[i].b = iheader.Colors[i].b;
	}

	//read pixels
	unsigned char * readtab = new unsigned char[iImgWidth*iImgHeight];
	obrazek.read((char *)readtab, fheader.Filesize-528-12);
	obrazek.close();

	pixels = new SDL_Color *[iImgHeight];
	for (int i = 0; i < iImgHeight; i++)
		pixels[i] = new SDL_Color[iImgWidth];

	Decompression(readtab, fheader.Filesize - 528 - 12);
	PixelTable();


	//if it's in greyscale show in that mode else in colormode
	if (fheader.Colormode == 0)
		greyscale();

	delete[] readtab;


	SDL_Flip(screen);
}

/*
void To7BitColor()
{

	//union which helps us to convert between types
	Transfer z;
	int c = 0, k = 0, found;

	if (tabletofile != nullptr)
		delete[] tabletofile;

	if (pxcolornumber != nullptr)
		delete[] pxcolornumber;
	pxcolornumber = new unsigned char[iImgWidth*iImgHeight];

	//memory alocation for saving to file
	tabletofile = new unsigned char[iImgWidth*iImgHeight];

	for (int i = 0; i < iImgHeight; i++)
	{
		for (int j = 0; j < iImgWidth; j++)
		{
			//make 7bit color
			pixels[i][j].r &= 0b11000000;
			pixels[i][j].g &= 0b11100000;
			pixels[i][j].b &= 0b11000000;

			//convert 24bit color to 7bit on 1 byte
			z.s.r = (pixels[i][j].r >> 6);
			z.s.g = (pixels[i][j].g >> 5);
			z.s.b = (pixels[i][j].b >> 6);

			//get only byte
			tabletofile[c] = z.a;
			c++;

			//show the difference - 7bit image
			setPixel(j, i, pixels[i][j].r, pixels[i][j].g, pixels[i][j].b);

			found = 0;
			for (unsigned int p = 0; p < tempnewpalette.size(); p++)
			{
				if (tempnewpalette[p] == pixels[i][j])
				{
					found = 1;
					pxcolornumber[i*iImgWidth + j] = p;
					break;
				}
			}

			if (found == 0)
			{
				tempnewpalette.push_back(pixels[i][j]);
				pxcolornumber[i*iImgWidth + j] = (unsigned char)(tempnewpalette.size() - 1);
			}

		}
	}
	cout << "Rozmiar palety " << tempnewpalette.size() << endl;
	for (unsigned int i = 0; i < tempnewpalette.size(); i++)
	{
		NewPalette[i].r = tempnewpalette[i].r;
		NewPalette[i].g = tempnewpalette[i].g;
		NewPalette[i].b = tempnewpalette[i].b;
	}
	tempnewpalette.clear();
	PixelTable();
	SDL_Flip(screen);
}
*/
void LoadFile()
{
	string a = GetFileName("Wybierz sciezke do pliku ");
	cout << "Wybrales  " << a << endl;

	if ( a.find(".exp") != string::npos )
		LoadExp(a);

	if (a.find(".bmp") != string::npos)
		loadBMP(a.c_str(), 0, 0);
}

string  GetFileName(const string & prompt) {
	const int BUFSIZE = 1024;
	char buffer[BUFSIZE] = { 0 };
	OPENFILENAMEA ofns = { 0 };
	ofns.lStructSize = sizeof(ofns);
	ofns.lpstrFile = buffer;
	ofns.lpstrFilter = "LZW Picture\0 * .EXP*\0BitMap\0 * .BMP\0";
	ofns.nMaxFile = BUFSIZE;
	ofns.lpstrTitle =  prompt.c_str();
	GetOpenFileNameA(&ofns);
	return buffer;

}

string GetFolderName()
{
	TCHAR szBuffer[MAX_PATH] = { 0 };
	BROWSEINFO bi;
	ZeroMemory(&bi, sizeof(BROWSEINFO));
	bi.hwndOwner = NULL;
	bi.pszDisplayName = szBuffer;
	bi.lpszTitle = _T("Select the file or folder from below:");
	bi.ulFlags = BIF_BROWSEINCLUDEFILES;
	LPITEMIDLIST idl = SHBrowseForFolder(&bi);
	if (NULL == idl)
	{
		return nullptr;
	}
	SHGetPathFromIDList(idl, szBuffer);
	char buffer[100];
	wcstombs(buffer, szBuffer, 100);
	string buf(buffer);
	return buffer;
}
