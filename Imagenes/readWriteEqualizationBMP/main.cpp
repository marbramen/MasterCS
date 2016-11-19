#include "BMP.h"
#include <cstdio>

#define N_IMAGE 16

int main(){
    string srcInput = "/home/cesar/CesarBragagnini/GitHub/MasterCS/Imagenes/readWriteEqualizationBMP/input/";
    string srcOutput = "/home/cesar/CesarBragagnini/GitHub/MasterCS/Imagenes/readWriteEqualizationBMP/output/";

    string arr[N_IMAGE] = {"ape_1bit.bmp", "clown_8bits.bmp", "BarbieTwins_8bits.bmp", "Einstein_8bits.bmp", "BarbieTwins_24bits.bmp", "clown_24bits.bmp", "Einstein_24bits.bmp", "Lena_1bit_2.bmp", "Lena_1bit.bmp","Lena_4bits_2.bmp","Lena_4bits.bmp", "Lena_8bits_2.bmp", "Lena_8bits.bmp", "Lena_24bits_2.bmp", "Lena_24bits.bmp", "Imagen_01_256.bmp"};

    for(int i = 0; i < N_IMAGE; i++){
        string nameFileIn = srcInput + arr[i];
        string nameFileOut = srcOutput + "equa_" + arr[i];
        printf("entra %s\n", nameFileIn.c_str());
        BMP myBMP;
        myBMP.loadImage(nameFileIn);
        myBMP.imprimirAtributos();
        // myBMP.imprimirPaleta();
        myBMP.equalizarImage();        
        myBMP.saveImage(nameFileOut);
    }        
    return 0;
}