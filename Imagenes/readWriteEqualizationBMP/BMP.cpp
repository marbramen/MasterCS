#ifndef BMP_CPP
#define BMP_CPP

#include "BMP.h"
#include <cstdio>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>

using namespace std;

void BMP::equalizarImage(){
    
    /* para BMP de 24 bits  y 8 bits si es 24 bits se crea 3 histogramas si es 8 bits se crea 1 histograma, para ambos casos se trabaja sobre el array data */
    if(channels == 24 || channels == 8){
        vector< vector<T_INT> > histoGr(type , vector<T_INT>(NUM_HISTO));                
        for(T_INT i = 0; i < rawDataImageSize / type; i++){
            for(T_INT k = 0; k < type; k++){
                histoGr[k][(T_INT)data[i*type + k]]++;            
            }
        }

        // calculamos el histograma acumulado
        vector< vector<T_INT> > resCum(type , vector<T_INT>(NUM_HISTO));
        for(T_INT k = 0; k < type; k++){
            resCum[k][0] = histoGr[k][0];
            for(T_INT i = 0; i < NUM_HISTO; i++){            
                resCum[k][i] = resCum[k][i-1] + histoGr[k][i];
            }
        }

        // generamos la ecualización
        T_DOUBLE nm = width * height;
        T_DOUBLE L = NUM_HISTO - 1.0;    
        for(T_INT i = 0; i < rawDataImageSize / type; i++)
            for(T_INT k = 0; k < type; k++)
                data[i*type + k] = (T_INT)(L / nm * (resCum[k][(T_INT)data[i*type + k]] * 1.0));
    }else{   
        /* para el 1,2,4 bits, se trabaja sobre dataRaw */     
        vector<T_DOUBLE> histoGr(paletteSize,  0);                
        vector<T_DOUBLE> resCum(paletteSize,  0);
        T_INT n = rawDataImageSize * BITS_IN_BYTE / channels;
        for(T_INT i = 0; i < n; i++){            
            histoGr[dataRaw[i]]++;
        }

        resCum[0] = histoGr[0];
        for(T_INT i = 1; i < paletteSize; i++){
            resCum[i] = resCum[i-1] + histoGr[i];
        }
            
        T_DOUBLE nm = n;
        T_DOUBLE L = paletteSize - 1.0;
        for(T_INT i = 0; i < n; i++)
            dataRaw[i] = ((L / nm *resCum[dataRaw[i]])+0.5);
    }    
}

void BMP::loadImage(string nameFile){
    printf("========= Load Image ======\n");
    nameImage = nameFile;
    FILE* file = fopen(nameFile.c_str(), "rb");
  	if (file == NULL){
          printf("la imagen no existe \n");
           return;
    }
     // read the 54-byte header
    fread(fileHeader, sizeof(T_UCHAR), FILEHEADER_SIZE, file);

    // extract fileHeaderData
    fileSize = abs(*(T_INT*)&fileHeader[2]);
    width = abs(*(T_INT*)&fileHeader[18]);
    height = *(T_INT*)&fileHeader[22];
    channels  =  *(T_INT*)&fileHeader[28];    
    type = (*(T_INT*)&fileHeader[28]) / BITS_IN_BYTE;
    rawDataImageSize = (*(T_INT*)&fileHeader[34]);

    rawDataImageSize = rawDataImageSize == 0 ?  width * height * channels / BITS_IN_BYTE : rawDataImageSize;
    
    if(channels == 1){
        paletteSize = 2;        
    }else if(channels == 4){
        paletteSize = 16;
    }else if(channels == 8){
        paletteSize = 256;
    }else
        paletteSize = 0;

    if(paletteSize != 0){
        palette = new paletteStruct[paletteSize];
        fread((T_UCHAR*)palette,sizeof(paletteStruct), paletteSize, file);
    }    

    data = new T_UCHAR[rawDataImageSize]; 
    fread(data, sizeof(T_UCHAR), rawDataImageSize, file);   
    fclose(file);
    /* creamos el dataRaw cuando la imagen BMP es menos de 8 bits
    ya que cada pixel esta representado por n bits y 
    la lectura siempre se hace con tipo de dato T_UCHAR (8bits), entonces solo se obtiene los bits necesarios para cada pixel */
    if(channels < 8){        
        dataRaw = new T_UCHAR[rawDataImageSize * BITS_IN_BYTE / channels];
        T_INT n = BITS_IN_BYTE / channels;
        for(T_INT i = 0; i < rawDataImageSize; i++)            
            for(T_INT j = 0; j < n; j++)
                dataRaw[i*n + j] = getXByIniFinBits(channels, j, data[i]);
    }
}

void BMP::saveImage(string nameFile){
    printf("========= Save Image ======\n");
    FILE* file = fopen(nameFile.c_str(), "wb");
    fwrite(fileHeader, sizeof(T_UCHAR),FILEHEADER_SIZE, file);
    if(paletteSize != 0)
        fwrite((T_UCHAR*)palette,sizeof(paletteStruct), paletteSize, file);    
    if(channels < 8){
        T_INT n = BITS_IN_BYTE / channels;
        for(T_INT i = 0; i < rawDataImageSize; i++){
            data[i] = dataRaw[i * n];
            for(T_INT j = 1; j < n; j++)
                data[i] = mergeBitsUChar(channels, data[i],dataRaw[i*n+j]);                                                    
        }
    }
    fwrite(data, sizeof(T_UCHAR), rawDataImageSize, file);  
    fclose(file);
}

void BMP::imprimirPaleta(){
    for(T_INT i = 0; i < paletteSize; i++){
        printf("R|G|B %d %d %d\n", (T_INT)palette[i].red, (T_INT)palette[i].green, (T_INT)palette[i].blue);
    }
}

void BMP::imprimirAtributos(){
    printf("Name Image: %s\n", nameImage.c_str());
    printf("width:%d , height:%d , fileSize:%d , type:%d , channels:%d  , rawDataImageSize:%d  \n", width, height, fileSize, type, channels, rawDataImageSize);
}

T_UCHAR BMP::getXByIniFinBits(T_INT cant, T_INT pos, T_UCHAR x){    
    x <<= cant * pos;
    x >>= (BITS_IN_BYTE - cant);
    return x;
}

T_UCHAR BMP::mergeBitsUChar(T_INT cant, T_UCHAR x, T_UCHAR aum){
    x <<= cant;    
    return x |aum;
}

#endif /*BMP_CPP*/