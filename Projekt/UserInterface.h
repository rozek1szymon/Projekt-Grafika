#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include "ImgInfo.h"

class UserInterface
{
    public:
        UserInterface();
        ImgInfo* Run();
        virtual ~UserInterface();

    private:
        ImgInfo* info;

};

#endif // USERINTERFACE_H
