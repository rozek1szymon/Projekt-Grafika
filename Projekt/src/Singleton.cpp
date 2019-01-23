#include "Singleton.h"

Singleton::Singleton()
{
    dedicatedColors = new SDL_Color[16];
}

Singleton::~Singleton()
{
    //dtor
}
