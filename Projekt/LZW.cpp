#include "LZW.h"

LZW::LZW()
{
    bitArray = new string[16];
    colors = new SDL_Color[16];
    BitGeneration();
}

LZW::~LZW()
{
    //dtor
}

void LZW::BitGeneration () {
    int numberOfElements = 4, numberOfSubsets = 0, numberOfDivisions, currentValue, counter = 0;
    int Set[numberOfElements + 1];
    string word;

    for (int i = 0; i <= numberOfElements; i++){
        Set[i] = 0;
    }

    do {
        word = "";

        for (int l = 1; l <= numberOfElements; l++) {
            word += to_string(Set[l]);
        }
        bitArray[counter] = word;
        dictionary.push_back(word);
        encodingDictionary.push_back(word);

        numberOfSubsets++;
        numberOfDivisions = 1;
        currentValue = numberOfSubsets;

        while (currentValue % 2 == 0) {
            currentValue = currentValue / 2;
            numberOfDivisions++;
        }

        if (numberOfDivisions <= numberOfElements) {
            Set[numberOfDivisions] = 1 - Set[numberOfDivisions];
        }

        counter++;

    } while (numberOfDivisions <= numberOfElements);
}

string LZW::takeOneColor (string colorCode, int index) {
    string oneCode = "";
    for (int i = 0; i < 4; i++) {
        oneCode += colorCode[i+4*index];
    }

    return oneCode;
}

void LZW::encodeLZW (SDL_Color** image, SDL_Color* colors, int width, int height) {
    cout<<"Encoding..."<<endl;
    rgb RGB;

    for (int i = 0; i < 16; i++) {
        RGB = {colors[i].r, colors[i].g, colors[i].b};
        encodeMap[RGB] = bitArray[i];
    }

    string outFile = "lzw_code.pgk";
    fstream out(outFile, ios::out);

    string result = "";

    if (out.good()) {

        string word = "";

        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                bool inDict = false;
                RGB = {image[i][j].r, image[i][j].g, image[i][j].b};
                string bits = encodeMap[RGB];
                string tempWord = word + bits;

                for (int d = 0; d < dictionary.size(); d++) {
                    if (dictionary[d] == tempWord) {
                        inDict = true;
                        word = tempWord;
                        break;
                    }
                }

                if (!inDict) {
                    it = find(dictionary.begin(), dictionary.end(), word);
                    auto index = distance(dictionary.begin(), it);
                    out << index << " ";
                    result += word;

                    dictionary.push_back(tempWord);
                    word = bits;
                }
            }
        }

        if (word != "") {
            it = find(dictionary.begin(), dictionary.end(), word);
            auto index = distance(dictionary.begin(), it);
            out << index;
            result += word;
        }

        out.close();
        cout << "Saved to: " + outFile << endl;
    } else {
        cout<<"Cannot save to file"<<endl;
    }
}

SDL_Color** LZW::decodeLZW(SDL_Color* colors, int width, int height) {
    cout<<"Decoding..."<<endl;
    fstream file;

    SDL_Color** decodedImage = new SDL_Color*[width];
    for (int i = 0; i < width; i++) {
        decodedImage[i] = new SDL_Color[height];
    }

    rgb RGB;
    for (int i = 0; i < 16; i++) {
        RGB = {colors[i].r, colors[i].g, colors[i].b};
        decodeMap[bitArray[i]] = RGB;
    }

    string word = "";
    string lastWord = "";

    file.open("lzw_code.pgk", ios::in);
    if(file.good())
    {
        int indexDict;
        file >> indexDict;
        word = encodingDictionary[indexDict];

        while (!file.eof()) {
            cout<<"loading..."<<endl;
            lastWord = encodingDictionary[indexDict];
            file >> indexDict;
            string tempWord;
            if (indexDict < encodingDictionary.size()) {
                tempWord = encodingDictionary[indexDict];
                encodingDictionary.push_back(lastWord + takeOneColor(tempWord, 0));
                word += tempWord;
            } else {
                tempWord = lastWord + takeOneColor(lastWord, 0);
                encodingDictionary.push_back(tempWord);
                word += (tempWord);
            }
        }

        file.close();

        cout<<"loaded"<<endl;

        rgb kolor;
        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                cout<<"copying..."<<endl;
                kolor = decodeMap[takeOneColor(word, (i*height+j))];
                //cout<<kolor.r<<" "<<kolor.g<<" "<<kolor.b<<endl;
                decodedImage[i][j] = {kolor.r, kolor.g, kolor.b};
            }
        }
        cout<<"done!"<<endl;
        return decodedImage;
    }
    else
    {
        exit(0);
    }
}
