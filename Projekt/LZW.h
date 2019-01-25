#ifndef LZW_H
#define LZW_H

#ifdef __APPLE__
#include <SDL/SDL.h>
#else
#include <SDL.h>
#endif

#include "iostream"
#include "fstream"
#include <algorithm>
#include <vector>
#include <map>
#include <string>

using namespace std;

struct rgb
{
    int r;
    int g;
    int b;

    bool const operator==(const rgb &col)
    {
        return r == col.r && g == col.g &&  b == col.b;
    }
};

struct compEncode
{
    bool operator() (const rgb &col, const rgb &col1) const
    {
        return col.r + col.g + col.b < col1.r + col1.g + col1.b;
    }
};

struct compDecode
{
    bool operator() (const string &s, const string &s1) const
    {
        return s < s1;
    }
};

class LZW
{
public:
    LZW();
    virtual ~LZW();

    void BitGeneration ();
    string takeOneColor (string colorCode, int index);
    void encodeLZW (SDL_Color** image, SDL_Color* colors, int width, int height);
    SDL_Color** decodeLZW (SDL_Color* colors, int width, int height);

private:
    vector<string> dictionary;
    vector<string>::iterator it;
    vector<string> encodingDictionary;

    map<rgb, string, compEncode> encodeMap;
    map<string, rgb, compDecode> decodeMap;

    string* bitArray;
    SDL_Color* colors;
};

#endif // LZW_H
