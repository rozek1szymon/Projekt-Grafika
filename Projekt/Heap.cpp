#include <iostream>
#include <ctime>
#include "Heap.h"

using namespace std;

static union Transfer
{
    char a;
    unsigned short l : 8;
};


void Heap::TopDown(int current)
{
    int left = current * 2 + 1, right = current * 2 + 2, greater = current;
    if (left < last && func(tab[greater], tab[left]))
        greater = left;
    if (right < last && func(tab[greater], tab[right]))
        greater = right;
    
    if (greater != current)
    {
        swap(tab[current], tab[greater]);
        TopDown(greater);
    }
}

void Heap::BottomUp(int current)
{
    int parent = (current - 1) / 2;
    if (parent < 0)
        return;
    
    if (func(tab[parent], tab[current]))
    {
        swap(tab[current], tab[parent]);
        BottomUp(parent);
    }
}

Heap::Heap(RLE * tab, int size2)
{
    size = size2;
    this->tab = tab;
    last = size2;
}

void Heap::Sortby(bool(*function)(RLE &, RLE&))
{
    func = function;
}

void Heap::HeapShow(int from, int to)
{
    Transfer z;
    for (int i = from; i < to; i++)
    {
        z.a = tab[i].r;
        cout << z.l << " ";
        z.a = tab[i].g;
        cout << z.l << " ";
        z.a = tab[i].b;
        cout << z.l << endl;
    }
    cout << endl;
}

void Heap::HeapMake()
{
    srand((unsigned int)time(NULL));
    for (int i = 0; i < size; i++)
    {
        tab[i].r = rand() % 256;
        tab[i].g = rand() % 256;
        tab[i].b = rand() % 256;
    }
    RestoreAtt();
}

void Heap::RestoreAtt()
{
    for (int i = size / 2 + 1; i >= 0; i--)
    {
        TopDown(i);
    }
}

void Heap::HeapSort()
{
    int nbuf = last;
    for (int i = size - 1; i > 0; i--)
    {
        swap(tab[0], tab[i]);
        last--;
        TopDown(0);
    }
    last = nbuf;
    
}

void Heap::insert(RLE element)
{
    tab[last] = element;
    BottomUp(last);
    last++;
}

RLE Heap::dequeue()
{
    if (last < 1)
        return tab[0];
    else
    {
        last--;
        swap(tab[last], tab[0]);
        TopDown(0);
        return tab[last];
    }
}

bool sortr(RLE &a, RLE &b)
{
    return a.r < b.r;
}
bool sortb(RLE &a, RLE &b)
{
    return a.b < b.b;
}
bool sortg(RLE &a, RLE &b)
{
    return a.g < b.g;
}

bool sortlum(RLE &a, RLE &b)
{
    int luma, lumb;
    luma = 0.02126*a.r + 0.7152*a.g + 0.0722*a.b;
    lumb = 0.02126*b.r + 0.7152*b.g + 0.0722*b.b;
    return luma < lumb;
}
