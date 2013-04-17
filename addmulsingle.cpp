#include "function.h"

void addsuchar2ucharaligned(uchar* src, short a,uchar* dest, int size)
{

	__m128i v = _mm_set1_epi16(a);
	__m128i z = _mm_setzero_si128();
	for(int i=0;i<size;i+=16)
	{
		__m128i a = _mm_load_si128((__m128i*)(src+i));

		//a  =_mm_cvtepu8_epi16(a);
		__m128i b = _mm_unpackhi_epi8(a,z);
		a = _mm_unpacklo_epi8(a,z);

		a = _mm_add_epi16(a,v);
		b = _mm_add_epi16(b,v);

		a = _mm_packus_epi16(a,b);
		_mm_store_si128((__m128i*)(dest+i),a);
	}
}

void mulsuchar2ucharaligned(uchar* src, short amp,uchar* dest, int size)
{
	__m128i v = _mm_set1_epi16(amp);
	__m128i z = _mm_setzero_si128();
	for(int i=0;i<size;i+=16)
	{
		__m128i a = _mm_load_si128((__m128i*)(src+i));

		__m128i b = _mm_unpackhi_epi8(a,z);
		a  =_mm_cvtepu8_epi16(a);

		a = _mm_mullo_epi16(a,v);
		b = _mm_mullo_epi16(b,v);

		a = _mm_packus_epi16(a,b);
		
		_mm_store_si128((__m128i*)(dest+i),a);
	}
}

void mulsuchar2shortaligned(uchar* src, short amp,short* dest, int size)
{

	__m128i v = _mm_set1_epi16(amp);
	__m128i z = _mm_setzero_si128();
	for(int i=0;i<size;i+=16)
	{
		__m128i a = _mm_load_si128((__m128i*)(src+i));

		//a  =_mm_cvtepu8_epi16(a);
		__m128i b = _mm_unpackhi_epi8(a,z);
		a = _mm_unpacklo_epi8(a,z);

		a = _mm_mullo_epi16(a,v);
		b = _mm_mullo_epi16(b,v);

		_mm_store_si128((__m128i*)(dest+i),a);
		_mm_store_si128((__m128i*)(dest+i+8),b);
	}
}

void addsuchar2shortaligned(uchar* src, short a,short* dest, int size)
{

	__m128i v = _mm_set1_epi16(a);
	__m128i z = _mm_setzero_si128();
	for(int i=0;i<size;i+=16)
	{
		__m128i a = _mm_load_si128((__m128i*)(src+i));

		//a  =_mm_cvtepu8_epi16(a);
		__m128i b = _mm_unpackhi_epi8(a,z);
		a = _mm_unpacklo_epi8(a,z);

		a = _mm_add_epi16(a,v);
		b = _mm_add_epi16(b,v);

		_mm_store_si128((__m128i*)(dest+i),a);
		_mm_store_si128((__m128i*)(dest+i+8),b);
	}
}

void mulsuchar2shortBF(uchar* src, short amp,short* dest, int size)
{
	for(int i=0;i<size;i++)
	{
		dest[i]=amp*src[i];
	}
}

void addsuchar2shortBF(uchar* src, short a,short* dest, int size)
{
	for(int i=0;i<size;i++)
	{
		dest[i]=a + (short)src[i] ;
	}
}

void addmullsingle()
{
	const int iter = 128*128*12;
	const int size = 256;
	_declspec( align(16) ) uchar data[size];
	_declspec( align(16) ) short dstshort[size];
	_declspec( align(16) ) uchar dstuchar[size];
	_declspec( align(16) ) int dstint[size];
	for(int i=0;i<size;i++)
	{
		data[i]=i;
		dstuchar[i]=0;
		dstshort[i]=0;
		dstint[i]=0;
	}

	int64 pre;

	clear(dstshort,size);
	pre = getTickCount();
	for(int i=0;i<iter;i++)
		addsuchar2shortBF(data,2,dstshort,size);
    cout<<"add s BF time: "<<1000.0*(getTickCount()-pre)/(getTickFrequency())<<" ms"<<endl;
	//printdataI(dstshort,size);

	clear(dstshort,size);
	pre = getTickCount();
	for(int i=0;i<iter;i++)
	addsuchar2shortaligned(data,2,dstshort,size);
    cout<<"add s SIMD time: "<<1000.0*(getTickCount()-pre)/(getTickFrequency())<<" ms"<<endl;
	//printdataI(dstshort,size);

	clear(dstshort,size);
	pre = getTickCount();
	for(int i=0;i<iter;i++)
	addsuchar2ucharaligned(data,2,dstuchar,size);
    cout<<"add s uchar SIMD time: "<<1000.0*(getTickCount()-pre)/(getTickFrequency())<<" ms"<<endl;


	clear(dstshort,size);
	pre = getTickCount();
	for(int i=0;i<iter;i++)
		mulsuchar2shortBF(data,2,dstshort,size);
    cout<<"mul s BF time: "<<1000.0*(getTickCount()-pre)/(getTickFrequency())<<" ms"<<endl;
	//printdataI(dstshort,size);
	
	clear(dstshort,size);
	pre = getTickCount();
	for(int i=0;i<iter;i++)
	mulsuchar2shortaligned(data,2,dstshort,size);
    cout<<"mul s SIMD time: "<<1000.0*(getTickCount()-pre)/(getTickFrequency())<<" ms"<<endl;
	//printdataI(dstshort,size);
	
	
	clear(dstuchar,size);
	pre = getTickCount();
	for(int i=0;i<iter;i++)
	mulsuchar2ucharaligned(data,2,dstuchar,size);
    cout<<"mul s uchar SIMD time: "<<1000.0*(getTickCount()-pre)/(getTickFrequency())<<" ms"<<endl;
	printdataI(dstuchar,size);
}