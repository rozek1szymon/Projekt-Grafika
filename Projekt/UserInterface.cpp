#include "UserInterface.h"
#include <iostream>

using namespace std;

UserInterface::UserInterface()
{
    info = new ImgInfo();
}

ImgInfo* UserInterface::Run() {
    string filename;
    int x;
    cout<<"[1] Konwertuj z .bmp do .pgkim"<<endl;
    cout<<"[2] Konwertuj z .pgkim do .bmp"<<endl;
    cin>>x;
    switch( x )
    {
    case 1:
        cout<<"Podaj nazwe pliku"<<endl;
        cin>>filename;
        info->filename = filename;
        cout<<"[1] Paleta barw: NARZUCONA"<<endl;
        cout<<"[2] Paleta barw: DEDYKOWANA"<<endl;
        cout<<"[3] Paleta barw: SKALA SZAROSCI"<<endl;
        cin>>x;
        if (x < 4 && x > 0) {
            info->paleta = x;
        } else {
            exit(0);
        }

        cout<<"[1] Dithering" <<endl;
        cout<<"[2] Brak ditheringu" <<endl;
        cin>>x;
        if (x == 1) {
            info->dithering = true;
        } else if (x == 2)  {
            info->dithering = false;
        } else {
            exit(0);
        }
        break;

    case 2:
        //jakiœ kod
        break;

    default:
        exit(0);
        break;
    }

    return info;
}

UserInterface::~UserInterface()
{

}
