#ifndef _FUNCTION_H_
#define _FUNCTION_H_
#include <opencv2/opencv.hpp>
#include <iostream>

#ifdef _DEBUG
#pragma comment(lib, "opencv_video232d.lib")
#pragma comment(lib, "opencv_ts232d.lib")
#pragma comment(lib, "opencv_objdetect232d.lib")
#pragma comment(lib, "opencv_ml232d.lib")
#pragma comment(lib, "opencv_legacy232d.lib")
#pragma comment(lib, "opencv_imgproc232d.lib")
#pragma comment(lib, "opencv_highgui232d.lib")
#pragma comment(lib, "opencv_haartraining_engine.lib")
#pragma comment(lib, "opencv_gpu232d.lib")
#pragma comment(lib, "opencv_flann232d.lib")
#pragma comment(lib, "opencv_features2d232d.lib")
#pragma comment(lib, "opencv_core232d.lib")
#pragma comment(lib, "opencv_contrib232d.lib")
#pragma comment(lib, "opencv_calib3d232d.lib")
#else
#pragma comment(lib, "opencv_video232.lib")
#pragma comment(lib, "opencv_ts232.lib")
#pragma comment(lib, "opencv_objdetect232.lib")
#pragma comment(lib, "opencv_ml232.lib")
#pragma comment(lib, "opencv_legacy232.lib")
#pragma comment(lib, "opencv_imgproc232.lib")
#pragma comment(lib, "opencv_highgui232.lib")
#pragma comment(lib, "opencv_haartraining_engine.lib")
#pragma comment(lib, "opencv_gpu232.lib")
#pragma comment(lib, "opencv_flann232.lib")
#pragma comment(lib, "opencv_features2d232.lib")
#pragma comment(lib, "opencv_core232.lib")
#pragma comment(lib, "opencv_contrib232.lib")
#pragma comment(lib, "opencv_calib3d232.lib")
#endif
using namespace cv;
using namespace std;

#include <nmmintrin.h> //MMX - SSE4.2 
#include <tmmintrin.h>
#include <smmintrin.h>
#include <intrin.h>      // MMX-SSE3命令セットを使用する場合インクルード   //
#include <emmintrin.h>   // MMX-SSE2命令セットを使用する場合インクルード   //
#include <xmmintrin.h>   // MMX-SSE命令セットを使用する場合インクルード    //
#include <mmintrin.h>    // MMX命令セットを使用する場合インクルード        //
//util
void checkCPU(void);

//test
void typecvtS2L();
void typecvtL2S();
void addmullsingle();
void shuffle_test();

//vsOpenCV
void vsOpenCV_copy();
void vsOpenCV_add();
void vsOpenCV_cvt();
void vsOpenCV_multiply();
void vsOpenCV_swapRB();
void vsOpenCV_BGR2Gray();
void vsOpenCV_split();
void vsOpenCV_flip();

template <class T>
void printdataI(T* data,const int size)
{
	for(int i=0;i<size;i++)
	{
		printf("%3d ",data[i]);
		if(i%16==15)printf("\n");
	}
	printf("\n");
}

template <class T>
void clear(T* data, int size )
{
	for(int i=0;i<size;i++)data[i]=0;
}

void printm128i(__m128i in, int num);
#endif 