#ifndef SINGLETON_H
#define SINGLETON_H

#ifdef __APPLE__
#include <SDL/SDL.h>
#else
#include <SDL.h>
#endif

class Singleton
{
    public:
        Singleton();
        virtual ~Singleton();
        SDL_Color* dedicatedColors;

    protected:

    private:
};

#endif // SINGLETON_H
