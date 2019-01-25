#ifndef IMGINFO_H
#define IMGINFO_H

#include <string>

class ImgInfo
{
    public:
        ImgInfo();
        virtual ~ImgInfo();

        std::string filename;
        int paleta;
        bool dithering;

    private:
};

#endif // IMGINFO_H
