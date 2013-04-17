#include "function.h"

//aligned
void cvtint2ucharAligned(int* src, uchar* dest, int size)
{
	for(int i=0;i<size;i+=16)
	{
		__m128i a = _mm_load_si128((__m128i*)(src+i));
		__m128i b = _mm_load_si128((__m128i*)(src+i+4));
		__m128i c = _mm_load_si128((__m128i*)(src+i+8));
		__m128i d = _mm_load_si128((__m128i*)(src+i+12));
		 a = _mm_packs_epi32(a,b);
		 c = _mm_packs_epi32(c,d);
		 a = _mm_packs_epi16(a,c);
		 
		 _mm_store_si128((__m128i*)(dest+i),a);
	}
}

void cvtshort2ucharAligned(short* src, uchar* dest, int size)
{
	for(int i=0;i<size;i+=16)
	{
		__m128i a = _mm_load_si128((__m128i*)(src+i));
		__m128i b = _mm_load_si128((__m128i*)(src+i+8));
		 a = _mm_packs_epi16(a,b);
		 _mm_store_si128((__m128i*)(dest+i),a);
	}
}


void cvtuchar2intAligned(uchar* src, int* dest, int size)
{
	for(int i=0;i<size;i+=16)
	{
		//load
		__m128i a = _mm_load_si128((__m128i*)(src+i));

		//cvt
		__m128i b  =_mm_cvtepu8_epi32(a);
		_mm_store_si128((__m128i*)(dest+i),b);
		//bit shift
		a = _mm_srli_si128(a,4);
		//cvt
		b  =_mm_cvtepu8_epi32(a);
		_mm_store_si128((__m128i*)(dest+i+4),b);
		a = _mm_srli_si128(a,4);
		b  =_mm_cvtepu8_epi32(a);
		_mm_store_si128((__m128i*)(dest+i+8),b);
		a = _mm_srli_si128(a,4);
		b  =_mm_cvtepu8_epi32(a);
		_mm_store_si128((__m128i*)(dest+i+12),b);
	}
}

void cvtuchar2shortAligned2(uchar* src, short* dest, int size)
{
	const __m128i zero = _mm_setzero_si128();
	for(int i=0;i<size;i+=16)
	{
		__m128i a = _mm_load_si128((__m128i*)(src+i));
		__m128i b = _mm_unpackhi_epi8(a,zero);

		a = _mm_unpacklo_epi8(a,zero);
		
		_mm_store_si128((__m128i*)(dest+i),a);
		_mm_store_si128((__m128i*)(dest+i+8),b);
	}
}

void cvtuchar2shortAligned(uchar* src, short* dest, int size)
{
	for(int i=0;i<size;i+=16)
	{
		__m128i a = _mm_load_si128((__m128i*)(src+i));
		__m128i b = _mm_srli_si128(a,8);

		a  =_mm_cvtepu8_epi16(a);		
		b  =_mm_cvtepu8_epi16(b);
		
		_mm_store_si128((__m128i*)(dest+i),a);
		_mm_store_si128((__m128i*)(dest+i+8),b);
	}
}

//not aligned

void cvtint2uchar(int* src, uchar* dest, int size)
{
	for(int i=0;i<size;i+=16)
	{
		__m128i a = _mm_loadu_si128((__m128i*)(src+i));
		__m128i b = _mm_loadu_si128((__m128i*)(src+i+4));
		__m128i c = _mm_loadu_si128((__m128i*)(src+i+8));
		__m128i d = _mm_loadu_si128((__m128i*)(src+i+12));
		 a = _mm_packus_epi32(a,b);
		 c = _mm_packus_epi32(c,d);
		 a = _mm_packus_epi16(a,c);
		 
		 _mm_storeu_si128((__m128i*)(dest+i),a);
	}
}

void cvtshort2uchar(short* src, uchar* dest, int size)
{
	for(int i=0;i<size;i+=16)
	{
		__m128i a = _mm_loadu_si128((__m128i*)(src+i));
		__m128i b = _mm_loadu_si128((__m128i*)(src+i+8));
		 a = _mm_packus_epi16(a,b);
		 _mm_storeu_si128((__m128i*)(dest+i),a);
	}
}

void cvtuchar2int(uchar* src, int* dest, int size)
{
	for(int i=0;i<size;i+=4)
	{
		__m128i a = _mm_loadu_si128((__m128i*)(src+i));

		a  =_mm_cvtepu8_epi32(a);
		_mm_store_si128((__m128i*)(dest+i),a);
	}
}
void cvtuchar2short(uchar* src, short* dest, int size)
{
	for(int i=0;i<size;i+=8)
	{
		__m128i a = _mm_loadu_si128((__m128i*)(src+i));
		 a  =_mm_cvtepu8_epi16(a);
		_mm_store_si128((__m128i*)(dest+i),a);
	}
}



//BF
void cvtuchar2shortBF(uchar* src, short* dest, int size)
{
		for(int i=0;i<size;i++)
	{
		dest[i] = (short)src[i];
	}
}
void cvtuchar2intBF(uchar* src, int* dest, int size)
{
		for(int i=0;i<size;i++)
	{
		dest[i] = (int)src[i];
	}
}
void cvtshort2ucharBF(short* src, uchar* dest, int size)
{
		for(int i=0;i<size;i++)
	{
		dest[i] = (uchar)src[i];
	}
}
void cvtint2ucharBF(int* src, uchar* dest, int size)
{
	for(int i=0;i<size;i++)
	{
		dest[i] = (uchar)src[i];
	}
}

void typecvtL2S()
{
	const int iter = 128*128*120;
	const int size = 256;
	_declspec( align(16) ) uchar data[size];
	_declspec( align(16) ) short data_s[size];
	_declspec( align(16) ) int data_i[size];
	_declspec( align(16) ) uchar dstuchar[size];
	_declspec( align(16) ) short dstshort[size];
	_declspec( align(16) ) int dstint[size];
	for(int i=0;i<size;i++)
	{
		data[i]=i;
		data_s[i]=i;
		data_i[i]=i;
		dstuchar[i]=0;
		dstshort[i]=0;
		dstint[i]=0;
	}

	int64 pre;
	//‘å‚«‚È‚à‚Ì‚©‚ç¬‚³‚È‚à‚Ì‚Ö
	clear(dstuchar,size);
	pre = getTickCount();
	for(int i=0;i<iter;i++)
		cvtshort2ucharBF(data_s,dstuchar,size);
    cout<<"short BF time: "<<1000.0*(getTickCount()-pre)/(getTickFrequency())<<" ms"<<endl;
	//printdataI(dstuchar,size);

	clear(dstuchar,size);
	pre = getTickCount();
	for(int i=0;i<iter;i++)
		cvtshort2uchar(data_s,dstuchar,size);
    cout<<"short N time: "<<1000.0*(getTickCount()-pre)/(getTickFrequency())<<" ms"<<endl;
	//printdataI(dstuchar,size);

	clear(dstuchar,size);
	pre = getTickCount();
	for(int i=0;i<iter;i++)
		cvtshort2ucharAligned(data_s,dstuchar,size);
    cout<<"short A time: "<<1000.0*(getTickCount()-pre)/(getTickFrequency())<<" ms"<<endl;
	//printdataI(dstuchar,size);

	
	clear(dstuchar,size);
	pre = getTickCount();
	for(int i=0;i<iter;i++)
		cvtint2ucharBF(data_i,dstuchar,size);
    cout<<"int BF time: "<<1000.0*(getTickCount()-pre)/(getTickFrequency())<<" ms"<<endl;
	//printdataI(dstuchar,size);
	clear(dstuchar,size);
	pre = getTickCount();
	for(int i=0;i<iter;i++)
		cvtint2uchar(data_i,dstuchar,size);
    cout<<"int N time: "<<1000.0*(getTickCount()-pre)/(getTickFrequency())<<" ms"<<endl;
	//printdataI(dstuchar,size);

	clear(dstuchar,size);
	pre = getTickCount();
	for(int i=0;i<iter;i++)
		cvtint2ucharAligned(data_i,dstuchar,size);
    cout<<"int A time: "<<1000.0*(getTickCount()-pre)/(getTickFrequency())<<" ms"<<endl;
	//printdataI(dstuchar,size);
}

//¬‚³‚È‚à‚Ì‚©‚ç‘å‚«‚È‚à‚Ì‚Ö
void typecvtS2L()
{
	const int iter = 128*128*120;
	const int size = 256;
	_declspec( align(16) ) uchar data[size];
	_declspec( align(16) ) short data_s[size];
	_declspec( align(16) ) int data_i[size];
	_declspec( align(16) ) uchar dstuchar[size];
	_declspec( align(16) ) short dstshort[size];
	_declspec( align(16) ) int dstint[size];
	for(int i=0;i<size;i++)
	{
		data[i]=i;
		data_s[i]=i;
		data_i[i]=i;
		dstuchar[i]=0;
		dstshort[i]=0;
		dstint[i]=0;
	}

	int64 pre;

	clear(dstshort,size);
	pre = getTickCount();
	for(int i=0;i<iter;i++)
	cvtuchar2shortBF(data,dstshort,size);
    cout<<"short BF time: "<<1000.0*(getTickCount()-pre)/(getTickFrequency())<<" ms"<<endl;

	clear(dstshort,size);
	pre = getTickCount();
	for(int i=0;i<iter;i++)
	cvtuchar2short(data,dstshort,size);
    cout<<"short N1 time: "<<1000.0*(getTickCount()-pre)/(getTickFrequency())<<" ms"<<endl;
	//printdataI(dstshort,size);

	clear(dstshort,size);
	pre = getTickCount();
	for(int i=0;i<iter;i++)
		cvtuchar2shortAligned(data,dstshort,size);
    cout<<"short A time: "<<1000.0*(getTickCount()-pre)/(getTickFrequency())<<" ms"<<endl;
	//printdataI(dstshort,size);
	

	clear(dstshort,size);
	pre = getTickCount();
	for(int i=0;i<iter;i++)
		cvtuchar2shortAligned2(data,dstshort,size);
    cout<<"short A2 time: "<<1000.0*(getTickCount()-pre)/(getTickFrequency())<<" ms"<<endl;
	printdataI(dstshort,size);


	pre = getTickCount();
	for(int i=0;i<iter;i++)
		cvtuchar2intBF(data,dstint,size);
    cout<<"int BF time: "<<1000.0*(getTickCount()-pre)/(getTickFrequency())<<" ms"<<endl;

	pre = getTickCount();
	for(int i=0;i<iter;i++)
		cvtuchar2int(data,dstint,size);
    cout<<"int N time: "<<1000.0*(getTickCount()-pre)/(getTickFrequency())<<" ms"<<endl;

	clear(dstint,size);
	pre = getTickCount();
	for(int i=0;i<iter;i++)
		cvtuchar2intAligned(data,dstint,size);
    cout<<"int A time: "<<1000.0*(getTickCount()-pre)/(getTickFrequency())<<" ms"<<endl;
	//printdataI(dstint,size);
}