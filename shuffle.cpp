#include "function.h"


void flipdataBF(uchar* src, uchar* dest, int size)
{
	for(int i=0;i<size;i++)
	{
		dest[i] = src[size-1-i];
	}
}
void flipdata(uchar* src, uchar* dest, const int size)
{
	const __m128i mask = _mm_setr_epi8(15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0);
	uchar* s = src+size-16;
	for(int i=0;i<size;i+=16)
	{
		__m128i a = _mm_load_si128((__m128i*)(s));
		a = _mm_shuffle_epi8(a,mask);
		_mm_store_si128((__m128i*)(dest+i),a);
		s-=16;
	}
}
void shuffle_test()
{
		const int iter = 128*128*12;
	const int size = 256;
	_declspec( align(16) ) uchar data[size];
	_declspec( align(16) ) uchar dstuchar[size];
	for(int i=0;i<size;i++)
	{
		data[i]=i;
	}


	int64 pre;

	clear(dstuchar,size);
	pre = getTickCount();
	for(int i=0;i<iter;i++)
		flipdataBF(data,dstuchar,size);
    cout<<"flip BF time: "<<1000.0*(getTickCount()-pre)/(getTickFrequency())<<" ms"<<endl;
	printdataI(dstuchar,size);

	clear(dstuchar,size);
	pre = getTickCount();
	for(int i=0;i<iter;i++)
		flipdata(data,dstuchar,size);
    cout<<"flip SSE time: "<<1000.0*(getTickCount()-pre)/(getTickFrequency())<<" ms"<<endl;
	printdataI(dstuchar,size);
}


/*
	__m128i z = _mm_setr_epi8(1,2,3,4,
		5,6,7,8,
		9,10,11,12,
		13,14,15,0);*/

