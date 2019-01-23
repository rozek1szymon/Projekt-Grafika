#ifndef Heap_h
#define Heap_h

#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <ctime>

#ifdef __APPLE__
#include <SDL/SDL.h>
#elif defined(linux)
#include <SDL/SDL.h>
#else
#include <SDL.h>
#endif

struct Transfer
{
    char a;
    unsigned short l : 8;
};

struct RLE
{
    int amount;
    Uint8 r, g, b;
    unsigned short i, j;
};

class Heap {
private:

    int size, last;
    RLE * tab;
    bool(*func)(RLE&, RLE&);
    void TopDown(int current);
    void BottomUp(int current);
public:
    Heap(RLE * tab, int size2);
    void Sortby(bool(*function)(RLE &, RLE&));
    void HeapShow(int from, int to);
    void HeapMake();
    void RestoreAtt();
    void HeapSort();
    void insert(RLE element);
    RLE dequeue();
};

bool sortr(RLE &a, RLE &b);
bool sortb(RLE &a, RLE &b);
bool sortg(RLE &a, RLE &b);
bool sortlum(RLE &a, RLE &b);


#endif /* Heap_h */
