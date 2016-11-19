#ifndef BMP_H
#define BMP_H

#include <string>
#include <vector>
#define FILEHEADER_SIZE 54
#define BITS_IN_BYTE 8
#define NUM_HISTO 256

using namespace std;

typedef unsigned char T_UCHAR ;
typedef int T_INT;
typedef double T_DOUBLE;

struct paletteStruct{
    T_UCHAR red;
    T_UCHAR green;
    T_UCHAR blue;
    T_UCHAR reserved;
};

class BMP{
    private:
        T_UCHAR fileHeader[FILEHEADER_SIZE];
        T_UCHAR* data;
        T_UCHAR* dataRaw;
        paletteStruct* palette;
        string nameImage;
        T_INT width;
        T_INT height;
        T_INT fileSize;
        T_INT type;
        T_INT channels;
        T_INT rawDataImageSize;
        T_INT paletteSize; 
        T_UCHAR getXByIniFinBits(T_INT,T_INT,T_UCHAR);
        T_UCHAR mergeBitsUChar(T_INT,T_UCHAR, T_UCHAR);

    public:
        void loadImage(string);
        void saveImage(string);        
        void equalizarImage();
        void imprimirAtributos();         
        void imprimirPaleta();
};

#endif /*BMP_H*/