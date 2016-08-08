/*
 * César Bragagnini
 * Detección de Bordes de forma serial, paralela usando CUDA
 * Curso: Programación Paralela
 * Se usa OpenCV para abrir y guardar las imagenes
 */
#include "opencv2/highgui/highgui.hpp"
#include <cstdio>
#include <time.h>

using namespace cv;

#define MAX_THREADS_BY_BLOCK 1024
#define DIM_BLOCK_X 32
#define DIM_BLOCK_Y 32
#define DIM_GRID_Y 10

int nRows, nCols;
inline int cmpByMaxUmbral(int x){ return x > 255 ? 255 : x; }
inline int myAbs(int x){ return x > 0 ? x : -1 * x;}
inline int gPos(int y, int x){ return y * nCols + x; }

/*
 * Se aplica filtro : X_HORIZONTAL, Y_VERTICAL
 * SobelX: SobX[3][3] = {{-1,0,1},{-2,0,2},{-1,0,2}}
 * SobelY: SobY[3][3] = {{1,2,1},{0,0,0},{-1,-2,-1}}
*/


void serialSobelAsImageIn1d(Mat inMatImage){
	Mat matImgFinal = inMatImage.clone();
	nRows = inMatImage.rows;
	nCols = inMatImage.cols;

	/*
	 * Considerar que la image se guarda en una matriz 1d
	 * se debe buscar la posicion correcta
	 */
	int* inImage = new int[nRows * nCols];
	for(int y = 0; y < nRows; y++)
		for(int x = 0; x < nCols; x++)
			inImage[gPos(y, x)] = inMatImage.at<uchar>(y,x);


	int* resImage = new int[nRows * nCols];

	clock_t time = clock();
	for(int y = 1; y < nRows - 1; y++){
		for(int x = 1; x < nCols - 1; x++){
			int valueX = (inImage[gPos(y-1, x+1)] + 2 * inImage[gPos(y, x+1)] + inImage[gPos(y+1, x+1)]) - (inImage[gPos(y-1, x-1)] + 2 * inImage[gPos(y, x-1)] + inImage[gPos(y+1, x-1)]);
			int valueY = (inImage[gPos(y-1, x-1)] + 2 * inImage[gPos(y-1, x)] + inImage[gPos(y-1, x+1)]) - (inImage[gPos(y+1, x-1)] + 2 * inImage[gPos(y+1, x)] + inImage[gPos(y+1, x+1)]);
			resImage[gPos(y, x)] = cmpByMaxUmbral(myAbs(valueX) + myAbs(valueY));
		}
	}
	time = clock() - time;

	for(int y = 0; y < nRows; y++)
		for(int x = 0; x < nCols; x++)
			matImgFinal.at<uchar>(y,x) = resImage[gPos(y, x)];
	delete(inImage);
	delete(resImage);
	imwrite("/home/cesar/CesarBragagnini/MCS/others/resSerialImage1d.png", matImgFinal);
	printf("termino filtro 1d en %f s.\n", ((float)(time))/CLOCKS_PER_SEC);
}


void serialSobelAsImageIn2d(Mat inMatImage){
    Mat matImgFinal = inMatImage.clone();
	nRows = inMatImage.rows;
	nCols = inMatImage.cols;

	/*
	 * La imagen se guarda en una matriz 2d
	 */
	int **inImage = new int*[nRows];
	for(int y = 0; y < nRows; y++){
		inImage[y] = new int[nCols];
		for(int x = 0; x < nCols; x++){
			inImage[y][x] = inMatImage.at<uchar>(y,x);
		}
	}

	int **resImag = new int*[nRows];
	resImag[0] = new int[nCols];

	clock_t time = clock();
	for(int y = 1; y < nRows - 1; y++){
		resImag[y] = new int[nCols];
		for(int x = 1; x < nCols - 1; x++){
			int valueX = (inImage[y-1][x+1] + 2 * inImage[y][x+1] + inImage[y+1][x+1]) - (inImage[y-1][x-1] + 2 * inImage[y][x-1] + inImage[y+1][x-1]) ;
			int valueY = (inImage[y-1][x-1] + 2 * inImage[y-1][x] + inImage[y-1][x+1]) - (inImage[y+1][x-1] + 2 * inImage[y+1][x] + inImage[y+1][x+1]) ;
			resImag[y][x] = cmpByMaxUmbral(myAbs(valueX) + myAbs(valueY));
		}
	}
	time = clock() - time;

    for(int y = 1; y < nRows - 1; y++){
    	for(int x = 1; x < nCols - 1; x++){
    		matImgFinal.at<uchar>(y, x) = resImag[y][x];
    	}
    }
    delete(inImage);
	imwrite("/home/cesar/CesarBragagnini/MCS/others/resSerialImage2d.png", matImgFinal);
    printf("termino filtro en 2d en %f  s.\n", ((float)(time))/CLOCKS_PER_SEC);
}

__device__ int cuGPos(int y, int x, int cuCols){
	return y * cuCols + x;
}

__global__ void cuda1ThreadByPixelWithGrid2dBlock2d(int *cuPoRows, int *cuPoCols, int *inImage, int *resImage){

	int inx = threadIdx.x + blockIdx.x * blockDim.x;
	int iny = threadIdx.y + blockIdx.y * blockDim.y;
	int threadIdGlob = inx + iny * gridDim.x;

	int cuRows = *cuPoRows;
	int cuCols = *cuPoCols;

	if(threadIdGlob < cuRows * cuCols){
		int y = threadIdGlob / cuCols;
		int x = threadIdGlob % cuCols;
		/*
		 * Aplicamos operador Sobel
		 * Tenemos que buscar en el array1d los vecinos de X,y
		*/
		int valueX = (inImage[cuGPos(y-1,x+1,cuCols)] + 2 * inImage[cuGPos(y,x+1,cuCols)] + inImage[cuGPos(y+1,x+1,cuCols)]) - (inImage[cuGPos(y-1,x-1,cuCols)] + 2 * inImage[cuGPos(y,x-1,cuCols)] + inImage[cuGPos(y+1,x-1,cuCols)]);
		int valueY = (inImage[cuGPos(y-1,x-1,cuCols)] + 2 * inImage[cuGPos(y-1,x,cuCols)] + inImage[cuGPos(y-1,x+1,cuCols)]) - (inImage[cuGPos(y+1,x-1,cuCols)] + 2 * inImage[cuGPos(y+1,x,cuCols)] + inImage[cuGPos(y+1,x+1,cuCols)]);
		valueX = valueX > 0 ? valueX : -1 * valueX;
		valueY = valueY > 0 ? valueY : -1 * valueY;
		resImage[cuGPos(y,x,cuCols)] = valueX  + valueY > 255 ? 255 : valueX + valueY;
	 }
}


__global__ void cuda1ThreadByPixelWithGrid1dBlock1d(int *cuPoRows, int *cuPoCols, int *inImage, int *resImage){

	int threadIdGlob = threadIdx.x + blockIdx.x * blockDim.x ;

	int cuRows = *cuPoRows;
	int cuCols = *cuPoCols;

	if(threadIdGlob < cuRows * cuCols){
		int y = threadIdGlob / cuCols;
		int x = threadIdGlob % cuCols;
		/*
		 * Aplicamos operador Sobel
		 * Tenemos que buscar en el array1d los vecinos de X,y
		*/
//		int valueX = (inImage[gPos(y-1, x+1)] + 2 * inImage[gPos(y, x+1)] + inImage[gPos(y+1, x+1)]) - (inImage[gPos(y-1, x-1)] + 2 * inImage[gPos(y, x-1)] + inImage[gPos(y+1, x-1)]);
//		int valueY = (inImage[gPos(y-1, x-1)] + 2 * inImage[gPos(y-1, x)] + inImage[gPos(y-1, x+1)]) - (inImage[gPos(y+1, x-1)] + 2 * inImage[gPos(y+1, x)] + inImage[gPos(y+1, x+1)]);
		int valueX = (inImage[cuGPos(y-1,x+1,cuCols)] + 2 * inImage[cuGPos(y,x+1,cuCols)] + inImage[cuGPos(y+1,x+1,cuCols)]) - (inImage[cuGPos(y-1,x-1,cuCols)] + 2 * inImage[cuGPos(y,x-1,cuCols)] + inImage[cuGPos(y+1,x-1,cuCols)]);
		int valueY = (inImage[cuGPos(y-1,x-1,cuCols)] + 2 * inImage[cuGPos(y-1,x,cuCols)] + inImage[cuGPos(y-1,x+1,cuCols)]) - (inImage[cuGPos(y+1,x-1,cuCols)] + 2 * inImage[cuGPos(y+1,x,cuCols)] + inImage[cuGPos(y+1,x+1,cuCols)]);
		valueX = valueX > 0 ? valueX : -1 * valueX;
		valueY = valueY > 0 ? valueY : -1 * valueY;
		resImage[cuGPos(y,x,cuCols)] = valueX  + valueY > 255 ? 255 : valueX + valueY;
	 }
}


void cudaSobelInLinearMemoryGrid1dBlock1d(Mat inMatImage){
	Mat matImgFinal = inMatImage.clone();
	nRows = inMatImage.rows;
	nCols = inMatImage.cols;

	cudaEvent_t start, stop;
	cudaEventCreate( &start );
	cudaEventCreate( &stop );
	float time;

	int* inImage = new int[nRows * nCols];
	for(int y = 0; y < nRows; y++)
		for(int x = 0; x < nCols; x++)
			inImage[gPos(y, x)] = inMatImage.at<uchar>(y,x);

	int *cuPoRows, *cuPoCols, *cuInImage, *cuResImage;
	cudaMalloc((void**) &cuPoRows, sizeof(int));
	cudaMalloc((void**) &cuPoCols, sizeof(int));
	cudaMalloc((void**) &cuInImage, nRows * nCols * sizeof(int));
	cudaMalloc((void**) &cuResImage, nRows * nCols * sizeof(int));

	cudaMemcpy(cuPoRows, &nRows, sizeof(int), cudaMemcpyHostToDevice);
	cudaMemcpy(cuPoCols, &nCols, sizeof(int), cudaMemcpyHostToDevice);
	cudaMemcpy(cuInImage, inImage, nRows * nCols * sizeof(int), cudaMemcpyHostToDevice);

	int N = nRows * nCols;
	dim3 blockDim(MAX_THREADS_BY_BLOCK,1,1);
	dim3 gridDim((N + MAX_THREADS_BY_BLOCK - 1) / MAX_THREADS_BY_BLOCK,1,1);

	cudaEventRecord( start, 0 );
	cuda1ThreadByPixelWithGrid1dBlock1d<<<gridDim, blockDim>>>(cuPoRows, cuPoCols, cuInImage, cuResImage);
	cudaEventRecord( stop, 0 );
	cudaEventSynchronize( stop );
	cudaEventElapsedTime( &time, start, stop );
	cudaEventDestroy( start );
	cudaEventDestroy( stop );

	cudaMemcpy(inImage, cuResImage, nRows * nCols * sizeof(int), cudaMemcpyDeviceToHost);
	cudaFree(cuPoRows);
	cudaFree(cuPoCols);
	cudaFree(cuInImage);
	cudaFree(cuResImage);

	for(int y = 1; y < nRows - 1; y++)
		for(int x = 1; x < nCols - 1; x++)
			matImgFinal.at<uchar>(y, x) = inImage[gPos(y, x)];
	delete(inImage);

	imwrite("/home/cesar/CesarBragagnini/MCS/others/resCudaImageGr1dBl1d.png", matImgFinal);
	printf("termino cuda Grid1d Block1d en %f s.\n", time / 1000.0);
}


void cudaSobelInLinearMemoryGrid2dBlock2d(Mat inMatImage, int heightGrid){
	Mat matImgFinal = inMatImage.clone();
	nRows = inMatImage.rows;
	nCols = inMatImage.cols;

	cudaEvent_t start, stop;
	cudaEventCreate( &start );
	cudaEventCreate( &stop );
	float time;

	int* inImage = new int[nRows * nCols];
	for(int y = 0; y < nRows; y++)
		for(int x = 0; x < nCols; x++)
			inImage[gPos(y, x)] = inMatImage.at<uchar>(y,x);

	int *cuPoRows, *cuPoCols, *cuInImage, *cuResImage;
	cudaMalloc((void**) &cuPoRows, sizeof(int));
	cudaMalloc((void**) &cuPoCols, sizeof(int));
	cudaMalloc((void**) &cuInImage, nRows * nCols * sizeof(int));
	cudaMalloc((void**) &cuResImage, nRows * nCols * sizeof(int));

	cudaMemcpy(cuPoRows, &nRows, sizeof(int), cudaMemcpyHostToDevice);
	cudaMemcpy(cuPoCols, &nCols, sizeof(int), cudaMemcpyHostToDevice);
	cudaMemcpy(cuInImage, inImage, nRows * nCols * sizeof(int), cudaMemcpyHostToDevice);

	int N = nRows * nCols;
	int nBloq = (N + MAX_THREADS_BY_BLOCK - 1) / MAX_THREADS_BY_BLOCK;

	dim3 blockDim(DIM_BLOCK_X,DIM_BLOCK_Y,1);
	dim3 gridDim((nBloq + heightGrid - 1) / heightGrid, heightGrid,1);

	cudaEventRecord( start, 0 );
	cuda1ThreadByPixelWithGrid2dBlock2d<<<gridDim, blockDim>>>(cuPoRows, cuPoCols, cuInImage, cuResImage);
	cudaEventRecord( stop, 0 );
	cudaEventSynchronize( stop );
	cudaEventElapsedTime( &time, start, stop );
	cudaEventDestroy( start );
	cudaEventDestroy( stop );

	cudaMemcpy(inImage, cuResImage, nRows * nCols * sizeof(int), cudaMemcpyDeviceToHost);
	cudaFree(cuPoRows);
	cudaFree(cuPoCols);
	cudaFree(cuInImage);
	cudaFree(cuResImage);

	for(int y = 1; y < nRows - 1; y++)
		for(int x = 1; x < nCols - 1; x++)
			matImgFinal.at<uchar>(y, x) = inImage[gPos(y, x)];

	delete(inImage);
	imwrite("/home/cesar/CesarBragagnini/MCS/others/resCudaImageGr2dBl2d.png", matImgFinal);
	printf("termino cuda en Grid2d Block2d gridY:%d %f s.\n",heightGrid, time / 1000.0);
}

int main( )
{
	/*
	 * Se usa OpenCV para leer y guardar imagenes,
	 * OpenCV considera
	 * al eje Y de arriba hacia abajo
	 * al eje X de izquierda a derecha
	*/
	int width[3] = {10, 30, 50};
	Mat inMatImage = imread("/home/cesar/CesarBragagnini/MCS/others/wallPaper04.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	imwrite("/home/cesar/CesarBragagnini/MCS/others/dgray.png", inMatImage);
	printf("Imagen %s n de %dx%d\n", "wallPaper04", inMatImage.rows, inMatImage.cols);
	/*
	 *  Sobel Filter
	 */
	serialSobelAsImageIn2d(inMatImage);
	serialSobelAsImageIn1d(inMatImage);
	cudaSobelInLinearMemoryGrid1dBlock1d(inMatImage);
	for(int j = 0; j < 3; j++)
		cudaSobelInLinearMemoryGrid2dBlock2d(inMatImage, width[j]);

    return 0;
}
