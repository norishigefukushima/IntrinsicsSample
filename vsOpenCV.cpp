#include "function.h"

#define SIZE Size(6400,4800)

void copy8SSE(const Mat& src, Mat& dest)
{
	const uchar* s = src.data;
	uchar* d = dest.data;
	const int size = src.size().area()*src.channels();
	for(int i=0;i<size;i+=16)
	{
		__m128i a = _mm_load_si128((__m128i*)(s+i));
		_mm_store_si128((__m128i*)(d+i),a);
		//_mm_stream_si128((__m128i*)(d+i),a);
	}
}
void copy8BF(const Mat& src, Mat& dest)
{
	const uchar* s = src.data;
	uchar* d = dest.data;
	const int size = src.size().area()*src.channels();
	for(int i=0;i<size;i++)
	{
		d[i]=s[i];
	}
}
void vsOpenCV_copy()
{
	const int iter = 10;
	
	Mat a(SIZE,CV_8U);
	Mat b(SIZE,CV_8U);
	
	int64 pre = getTickCount();
	for(int i=0;i<iter;i++)
	{
		a.copyTo(b);
		//b.copyTo(a);
	}
	cout<<"opencv time: "<<1000.0*(getTickCount()-pre)/(getTickFrequency())<<" ms"<<endl;

	pre = getTickCount();
	for(int i=0;i<iter;i++)
	{
		copy8BF(a,b);
		//copy8BF(b,a);
	}
	cout<<"normal time: "<<1000.0*(getTickCount()-pre)/(getTickFrequency())<<" ms"<<endl;

	pre = getTickCount();
	for(int i=0;i<iter;i++)
	{
		copy8SSE(a,b);
		//copy8SSE(b,a);
	}
	cout<<"SSE time: "<<1000.0*(getTickCount()-pre)/(getTickFrequency())<<" ms"<<endl;
}

void add8SSE(Mat& a, Mat& b, Mat& c)
{
	uchar* pa = a.data;
	uchar* pb = b.data;
	uchar* pc = c.data;
	const int size = a.size().area()*a.channels();

	for(int i=0;i<size;i+=16)
	{
		__m128i a = _mm_load_si128((__m128i*)(pa+i));
		__m128i b = _mm_load_si128((__m128i*)(pb+i));

		a = _mm_adds_epu8(a,b);
		_mm_stream_si128((__m128i*)(pc+i),a);
	}
}
void add8BF(Mat& a, Mat& b, Mat& c)
{
	uchar* pa = a.data;
	uchar* pb = b.data;
	uchar* pc = c.data;
	const int size = a.size().area()*a.channels();
	for(int i=0;i<size;i++)
	{

		pc[i]=pa[i]+pb[i];
	}
}

void vsOpenCV_add()
{
	const int iter = 10;
	
	Mat a(SIZE,CV_8U);
	Mat b(SIZE,CV_8U);
	Mat c(SIZE,CV_8U);

	int64 pre = getTickCount();
	for(int i=0;i<iter;i++)
	{
		cv::add(a,b,c);
	}
	cout<<"opencv time: "<<1000.0*(getTickCount()-pre)/(getTickFrequency())<<" ms"<<endl;

	pre = getTickCount();
	for(int i=0;i<iter;i++)
		add8BF(a,b,c);
	cout<<"normal time: "<<1000.0*(getTickCount()-pre)/(getTickFrequency())<<" ms"<<endl;

	pre = getTickCount();
	for(int i=0;i<iter;i++)
		add8SSE(a,b,c);
	cout<<"SSE time: "<<1000.0*(getTickCount()-pre)/(getTickFrequency())<<" ms"<<endl;
}


void multiply8SSE(const Mat& src1, const Mat& src2,Mat& dest)
{
	const uchar* s1 = src1.data;
	const uchar* s2 = src2.data;
	uchar* d = dest.data;
	const int size = src1.size().area()*src1.channels();

	const __m128i zero = _mm_setzero_si128();
	for(int i=0;i<size;i+=16)
	{
		__m128i a = _mm_load_si128((__m128i*)(s1+i));
		__m128i b = _mm_load_si128((__m128i*)(s2+i));

		__m128i a1 = _mm_unpackhi_epi8(a,zero);
		__m128i a2 = _mm_unpacklo_epi8(a,zero);

		__m128i b1 = _mm_unpackhi_epi8(b,zero);
		__m128i b2 = _mm_unpacklo_epi8(b,zero);

		a = _mm_mullo_epi16(a1,b1);
		b = _mm_mullo_epi16(a2,b2);

		a = _mm_packus_epi16(a,b);
		
		_mm_store_si128((__m128i*)(d+i),a);
	}
}
void multiply8BF(Mat& a, Mat& b, Mat& c)
{
	uchar* pa = a.data;
	uchar* pb = b.data;
	uchar* pc = c.data;
	const int size = a.size().area()*a.channels();
	for(int i=0;i<size;i++)
	{
		pc[i]=saturate_cast<uchar>(pa[i]*pb[i]);
	}
}

void vsOpenCV_multiply()
{
	const int iter = 10;
	
	Mat a(SIZE,CV_8U);
	Mat b(SIZE,CV_8U);
	Mat c(SIZE,CV_8U);

	a=20;
	b=10;
	
	int64 pre = getTickCount();
	for(int i=0;i<iter;i++)
		cv::multiply(a,b,c);
	cout<<"opencv time: "<<1000.0*(getTickCount()-pre)/(getTickFrequency())<<" ms"<<endl;

	c.setTo(0);
	pre = getTickCount();
	for(int i=0;i<iter;i++)
		multiply8BF(a,b,c);
	cout<<"normal time: "<<1000.0*(getTickCount()-pre)/(getTickFrequency())<<" ms"<<endl;

	c.setTo(0);
	pre = getTickCount();
	for(int i=0;i<iter;i++)
		multiply8SSE(a,b,c);
	cout<<"SSE time: "<<1000.0*(getTickCount()-pre)/(getTickFrequency())<<" ms"<<endl;
	
}

void cvtuchar2ushortMatBF(Mat& a,Mat& b)
{
	const int size = a.size().area()*a.channels();
	const uchar* s = a.ptr<uchar>(0);
	unsigned short* d = b.ptr<unsigned short>(0);
	for(int i=0;i<size;i++)
	{
		d[i]=(unsigned short)s[i];
	}
}

void convertTouchar2ushortMatBF(Mat& src,Mat& dest, int alpha, int beta)
{
	const int size = src.size().area()*src.channels();
	const uchar* s = src.ptr<uchar>(0);
	unsigned short* d = dest.ptr<unsigned short>(0);
	const unsigned short a_ = (short) alpha;
	const unsigned short b_ = (short) beta;
	for(int i=0;i<size;i++)
	{
		d[i]=(unsigned short)(a_*s[i] + b_);
	}
}
void cvtuchar2ushortMatSSE(Mat& a,Mat& b)
{
	const int size = a.size().area()*a.channels();	
	const uchar* s = a.ptr<uchar>(0);
	unsigned short* d = b.ptr<unsigned short>(0);

	__m128i zero = _mm_setzero_si128();
	for(int i=0;i<size;i+=16)
	{		
		__m128i a = _mm_load_si128((__m128i*)(s+i));		
		__m128i b = _mm_unpackhi_epi8(a,zero);
		
		a = _mm_unpacklo_epi8(a,zero);
		
		_mm_store_si128((__m128i*)(d+i),a);
		_mm_store_si128((__m128i*)(d+i+8),b);
		//_mm_stream_si128((__m128i*)(d+i),a);
		//_mm_stream_si128((__m128i*)(d+i+8),b);
	}
}

void convertTouchar2ushortMatSSE(Mat& src,Mat& dest, const int alpha, const int beta)
{
	const int size = src.size().area()*src.channels();	
	const uchar* s = src.ptr<uchar>(0);
	unsigned short* d = dest.ptr<unsigned short>(0);

	const __m128i zero = _mm_setzero_si128();
	const __m128i A = _mm_set1_epi16(alpha);
	const __m128i B = _mm_set1_epi16(beta);
	for(int i=0;i<size;i+=16)
	{		
		__m128i a = _mm_load_si128((__m128i*)(s+i));		
		__m128i b = _mm_unpackhi_epi8(a,zero);
		b = _mm_mullo_epi16(b,A);
		
		a = _mm_unpacklo_epi8(a,zero);
		a = _mm_mullo_epi16(a,A);
		a = _mm_adds_epi16(a,B);
		b = _mm_adds_epi16(b,B);
		
		_mm_store_si128((__m128i*)(d+i),a);
		_mm_store_si128((__m128i*)(d+i+8),b);
		//_mm_stream_si128((__m128i*)(d+i),a);
		//_mm_stream_si128((__m128i*)(d+i+8),b);
	}
}
void vsOpenCV_cvt()
{
	const int iter = 10;
	
	Mat a(SIZE,CV_8U);
	//Mat a(Size(640,480),CV_8U);
	Mat b(a.size(),CV_16U);

	a.setTo(100);
	int64 pre = getTickCount();
	for(int i=0;i<iter;i++)
		a.convertTo(b,CV_16U);
	cout<<"opencv time: "<<1000.0*(getTickCount()-pre)/(getTickFrequency())<<" ms"<<endl;

	pre = getTickCount();
	for(int i=0;i<iter;i++)
		a.convertTo(b,CV_16U,2,10);
	cout<<"opencv ax+b time: "<<1000.0*(getTickCount()-pre)/(getTickFrequency())<<" ms"<<endl;

	pre = getTickCount();
	for(int i=0;i<iter;i++)
		cvtuchar2ushortMatBF(a,b);
	cout<<"normal time: "<<1000.0*(getTickCount()-pre)/(getTickFrequency())<<" ms"<<endl;

	pre = getTickCount();
	for(int i=0;i<iter;i++)
		convertTouchar2ushortMatBF(a,b,2,10);
	cout<<"normal ax+b time: "<<1000.0*(getTickCount()-pre)/(getTickFrequency())<<" ms"<<endl;

	pre = getTickCount();
	for(int i=0;i<iter;i++)
		cvtuchar2ushortMatSSE(a,b);
	cout<<"SSE time: "<<1000.0*(getTickCount()-pre)/(getTickFrequency())<<" ms"<<endl;

	pre = getTickCount();
	for(int i=0;i<iter;i++)
		convertTouchar2ushortMatSSE(a,b,2,10);
	cout<<"SSE ax+b time: "<<1000.0*(getTickCount()-pre)/(getTickFrequency())<<" ms"<<endl;
	//Mat c; b.convertTo(c,CV_8U);imshow("b",c);waitKey();


}

void cvtBGR2RGB_BF(Mat& a,Mat& b)
{
	const int size = a.size().area()*a.channels();
	const uchar* s = a.ptr<uchar>(0);
	uchar* d = b.ptr<uchar>(0);
	for(int i=0;i<size;i+=3)
	{
		const uchar v=s[i+0];
		const uchar v2=s[i+1];
		const uchar v3=s[i+2];
		d[i+2]=v;
		d[i+1]=v2;
		d[i+0]=v3;
	}
}

void cvtBGR2RGB_SSE_nonal(Mat& src,Mat& dest)
{
	const int size = src.size().area()*src.channels();
	const uchar* s = src.data;
	uchar* d = dest.data;

	//BGRBGRBGRBGR BGRB
	//to
	//RGBRGBRGBRGB BGRB
	__m128i mask = _mm_setr_epi8
		(2,1,0,
		5,4,3,
		8,7,6,
		11,10,9,
		14,13,12,
		15);//ここを捨てる
	for(int i=0;i<size;i+=15)
	{
		__m128i a = _mm_loadu_si128((__m128i*)(s+i));
		a = _mm_shuffle_epi8(a,mask);
		_mm_storeu_si128((__m128i*)(d+i),a);
	}
}

void cvtBGR2RGB_SSE(Mat& src,Mat& dest)
{
	const int size = src.size().area()*src.channels();
	const uchar* s = src.ptr<uchar>(0);
	uchar* d = dest.ptr<uchar>(0);

	//BGR BGR BGR BGR BGR   B
	//GR BGR BGR BGR BGR BG
	//R BGR BGR BGR BGR BGR

	__m128i mask1 = _mm_setr_epi8
		(2,1,0,
		5,4,3,
		8,7,6,
		11,10,9,
		14,13,12,
		15);

	__m128i mask2 = _mm_setr_epi8
		(0,14,
		4,3,2,
		7,6,5,
		10,9,8,
		13,12,14,
		1,15);

	__m128i mask3 = _mm_setr_epi8
		(0,
		3,2,1,
		6,5,4,
		9,8,7,
		12,11,10,
		15,14,13
		);
	
	__m128i a,b,c;
	int Ba,Rb,Bb,Rc;
	for(int i=0;i<size;i+=48)
	{
		a = _mm_load_si128((__m128i*)(s+i));
		b = _mm_load_si128((__m128i*)(s+i+16));
		c = _mm_load_si128((__m128i*)(s+i+32));

		Ba = _mm_extract_epi8(a,15);
		Rb = _mm_extract_epi8(b,1);
		Bb = _mm_extract_epi8(b,14);
		Rc = _mm_extract_epi8(c,0);

		a = _mm_shuffle_epi8(a,mask1);
		b = _mm_shuffle_epi8(b,mask2);
		c = _mm_shuffle_epi8(c,mask3);

		a=_mm_insert_epi8(a,Rb,15);
		b=_mm_insert_epi8(b,Ba,1);
		b=_mm_insert_epi8(b,Rc,14);
		c=_mm_insert_epi8(c,Bb,0);
		
		_mm_stream_si128((__m128i*)(d+i),a);
		_mm_stream_si128((__m128i*)(d+i+16),b);
		_mm_stream_si128((__m128i*)(d+i+32),c);
	}
}


void vsOpenCV_swapRB()
{
	const int iter = 5;
	
	Mat a(SIZE,CV_8UC3);
	a= CV_RGB(255,0,0);
	Mat b(a.size(),CV_8UC3);
	
	int64 pre = getTickCount();
	for(int i=0;i<iter;i++)
		cvtColor(a,b,CV_BGR2RGB);
	cout<<"opencv time: "<<1000.0*(getTickCount()-pre)/(getTickFrequency())<<" ms"<<endl;

	pre = getTickCount();
	for(int i=0;i<iter;i++)
		cvtBGR2RGB_BF(a,b);
	cout<<"normal time: "<<1000.0*(getTickCount()-pre)/(getTickFrequency())<<" ms"<<endl;

	b.setTo(0);
	pre = getTickCount();
	for(int i=0;i<iter;i++)
		cvtBGR2RGB_SSE_nonal(a,b);
	cout<<"SSE time: "<<1000.0*(getTickCount()-pre)/(getTickFrequency())<<" ms"<<endl;
	
	b.setTo(0);
	pre = getTickCount();
	for(int i=0;i<iter;i++)
		cvtBGR2RGB_SSE(a,b);
	cout<<"SSE time: "<<1000.0*(getTickCount()-pre)/(getTickFrequency())<<" ms"<<endl;
}



void splitBF(Mat& src, vector<Mat>& dest)
{
		const int size = src.size().area();
	uchar* s = src.ptr<uchar>(0);
	uchar* b = dest[0].ptr<uchar>(0);
	uchar* g = dest[1].ptr<uchar>(0);
	uchar* r = dest[2].ptr<uchar>(0);

	
	for(int i=0;i<size;i++)
	{
		*b=s[0];
		*g=s[1];
		*r=s[2];
		s+=3;r++;g++;b++;
	}
}


//整理した関数
void splitSSE__(Mat& src, vector<Mat>& dest)
{
	const int size = src.size().area()*src.channels();
	uchar* s = src.ptr<uchar>(0);
	uchar* B = dest[0].ptr<uchar>(0);
	uchar* G = dest[1].ptr<uchar>(0);
	uchar* R = dest[2].ptr<uchar>(0);

	//BGR BGR BGR BGR BGR   B
	//GR BGR BGR BGR BGR BG
	//R BGR BGR BGR BGR BGR

	//BBBBBBGGGGGRRRRRへシャッフル
	const __m128i mask1 = _mm_setr_epi8(0,3,6,9,12,15,1,4,7,10,13,2,5,8,11,14);
	const __m128i smask1 = _mm_setr_epi8(6,7,8,9,10,0,1,2,3,4,5,11,12,13,14,15);
	const __m128i ssmask1 = _mm_setr_epi8(11,12,13,14,15,0,1,2,3,4,5,6,7,8,9,10);

	//GGGGGGBBBBBRRRRRへシャッフル
	const __m128i mask2 = _mm_setr_epi8(0,3,6,9,12,15, 2,5,8,11,14,1,4,7,10,13);
	//const __m128i smask2 = _mm_setr_epi8(6,7,8,9,10,0,1,2,3,4,5,11,12,13,14,15);
	const __m128i ssmask2 = _mm_setr_epi8(0,1,2,3,4,11,12,13,14,15,5,6,7,8,9,10);

	//RRRRRRGGGGGBBBBBへシャッフル
	//__m128i mask3 = _mm_setr_epi8(0,3,6,9,12,15, 2,5,8,11,14,1,4,7,10,13);
	//const __m128i smask3 = _mm_setr_epi8(6,7,8,9,10,0,1,2,3,4,5,6,7,8,9,10);
	//const __m128i ssmask3 = _mm_setr_epi8(11,12,13,14,15,0,1,2,3,4,5,6,7,8,9,10);

	const __m128i bmask1 = _mm_setr_epi8
		(255,255,255,255,255,255,0,0,0,0,0,0,0,0,0,0);

	const __m128i bmask2 = _mm_setr_epi8
		(255,255,255,255,255,255,255,255,255,255,255,0,0,0,0,0);

	const __m128i bmask3 = _mm_setr_epi8
		(255,255,255,255,255,0,0,0,0,0,0,0,0,0,0,0);

	const __m128i bmask4 = _mm_setr_epi8
		(255,255,255,255,255,255,255,255,255,255,0,0,0,0,0,0);	
	
	__m128i a,b,c;
	for(int i=0;i<size;i+=48)
	{	
		a = _mm_shuffle_epi8(_mm_load_si128((__m128i*)(s+i)),mask1);
		b = _mm_shuffle_epi8(_mm_load_si128((__m128i*)(s+i+16)),mask2);
		c = _mm_shuffle_epi8(_mm_load_si128((__m128i*)(s+i+32)),mask2);
		
		_mm_stream_si128((__m128i*)(B),_mm_blendv_epi8(c,_mm_blendv_epi8(b,a,bmask1),bmask2));
		
		a = _mm_shuffle_epi8(a,smask1);
		b = _mm_shuffle_epi8(b,smask1);
		c = _mm_shuffle_epi8(c,smask1);
		_mm_stream_si128((__m128i*)(G),_mm_blendv_epi8(c,_mm_blendv_epi8(b,a,bmask3),bmask2));

		a = _mm_shuffle_epi8(a,ssmask1);
		c = _mm_shuffle_epi8(c,ssmask1);
		b = _mm_shuffle_epi8(b,ssmask2);
				
		_mm_stream_si128((__m128i*)(R),_mm_blendv_epi8(c,_mm_blendv_epi8(b,a,bmask3),bmask4));

		B+=16;G+=16;R+=16;
	}
}

void splitSSE(Mat& src, vector<Mat>& dest)
{
	const int size = src.size().area()*src.channels();
	uchar* s = src.ptr<uchar>(0);
	uchar* B = dest[0].ptr<uchar>(0);
	uchar* G = dest[1].ptr<uchar>(0);
	uchar* R = dest[2].ptr<uchar>(0);

	//BGR BGR BGR BGR BGR   B
	//GR BGR BGR BGR BGR BG
	//R BGR BGR BGR BGR BGR

	//BBBBBBGGGGGRRRRRへシャッフル
	const __m128i mask1 = _mm_setr_epi8(0,3,6,9,12,15,1,4,7,10,13,2,5,8,11,14);
	const __m128i smask1 = _mm_setr_epi8(6,7,8,9,10,0,1,2,3,4,5,11,12,13,14,15);
	const __m128i ssmask1 = _mm_setr_epi8(11,12,13,14,15,0,1,2,3,4,5,6,7,8,9,10);

	//GGGGGGBBBBBRRRRRへシャッフル
	const __m128i mask2 = _mm_setr_epi8(0,3,6,9,12,15, 2,5,8,11,14,1,4,7,10,13);
	//const __m128i smask2 = _mm_setr_epi8(6,7,8,9,10,0,1,2,3,4,5,11,12,13,14,15);
	const __m128i ssmask2 = _mm_setr_epi8(0,1,2,3,4,11,12,13,14,15,5,6,7,8,9,10);

	//RRRRRRGGGGGBBBBBへシャッフル
	//__m128i mask3 = _mm_setr_epi8(0,3,6,9,12,15, 2,5,8,11,14,1,4,7,10,13);
	//const __m128i smask3 = _mm_setr_epi8(6,7,8,9,10,0,1,2,3,4,5,6,7,8,9,10);
	//const __m128i ssmask3 = _mm_setr_epi8(11,12,13,14,15,0,1,2,3,4,5,6,7,8,9,10);

	const __m128i bmask1 = _mm_setr_epi8
		(255,255,255,255,255,255,0,0,0,0,0,0,0,0,0,0);

	const __m128i bmask2 = _mm_setr_epi8
		(255,255,255,255,255,255,255,255,255,255,255,0,0,0,0,0);

	const __m128i bmask3 = _mm_setr_epi8
		(255,255,255,255,255,0,0,0,0,0,0,0,0,0,0,0);

	const __m128i bmask4 = _mm_setr_epi8
		(255,255,255,255,255,255,255,255,255,255,0,0,0,0,0,0);	
	
	__m128i a,b,c;
	for(int i=0;i<size;i+=48)
	{
		a = _mm_stream_load_si128((__m128i*)(s+i));
		b = _mm_stream_load_si128((__m128i*)(s+i+16));
		c = _mm_stream_load_si128((__m128i*)(s+i+32));
		a = _mm_load_si128((__m128i*)(s+i));
		b = _mm_load_si128((__m128i*)(s+i+16));
		c = _mm_load_si128((__m128i*)(s+i+32));
	
		__m128i v = _mm_blendv_epi8(b,a,bmask1);
		v = _mm_blendv_epi8(c,v,bmask2);
		_mm_stream_si128((__m128i*)(B),v);
		
		a = _mm_shuffle_epi8(a,smask1);
		b = _mm_shuffle_epi8(b,smask1);
		c = _mm_shuffle_epi8(c,smask1);
		v = _mm_blendv_epi8(b,a,bmask3);
		v = _mm_blendv_epi8(c,v,bmask2);
		_mm_stream_si128((__m128i*)(G),v);

		a = _mm_shuffle_epi8(a,ssmask1);
		c = _mm_shuffle_epi8(c,ssmask1);
		b = _mm_shuffle_epi8(b,ssmask2);
		
		v = _mm_blendv_epi8(b,a,bmask3);
		v = _mm_blendv_epi8(c,v,bmask4);
		_mm_stream_si128((__m128i*)(R),v);

		B+=16;G+=16;R+=16;
	}
}
void vsOpenCV_split()
{
	const int iter = 10;
	
	Mat a(SIZE,CV_8UC3);
	a= CV_RGB(64,128,255);
	//Mat a = imread("lenna.png");
	
	vector<Mat> v(4);
	v[0].create(a.size(),CV_8U);
	v[1].create(a.size(),CV_8U);
	v[2].create(a.size(),CV_8U);
	v[3].create(a.size(),CV_8U);
	int64 pre = getTickCount();
	
	for(int i=0;i<iter;i++)
	{
		split(a,v);
	}
	cout<<"opencv time: "<<1000.0*(getTickCount()-pre)/(getTickFrequency())<<" ms"<<endl;

	pre = getTickCount();
	for(int i=0;i<iter;i++)
		splitBF(a,v);
	cout<<"normal time: "<<1000.0*(getTickCount()-pre)/(getTickFrequency())<<" ms"<<endl;
	//imshow("a",v[0]);waitKey();

	v[0].setTo(0);
	v[1].setTo(0);
	v[2].setTo(0);
	pre = getTickCount();
	for(int i=0;i<iter;i++)
		splitSSE__(a,v);
	cout<<"SSE time: "<<1000.0*(getTickCount()-pre)/(getTickFrequency())<<" ms"<<endl;
	//imshow("a",v[ch]);waitKey();

	for(int ch=0;ch<3;ch++)
	{
	for(int i=0;i<16;i++)
	{
		cout<<(int)v[ch].data[i]<<",";
	}
	cout<<endl;
	}
}

void cvtBGR2GraySSEShort(Mat& src, Mat& dest)
{
	const int size = src.size().area()*src.channels();
	uchar* s = src.ptr<uchar>(0);
	uchar* d = dest.ptr<uchar>(0);

	//データ構造
	//BGR BGR BGR BGR BGR   B
	//GR BGR BGR BGR BGR BG
	//R BGR BGR BGR BGR BGR
	//BBBBBBGGGGGRRRRRへシャッフル
	const __m128i mask1 = _mm_setr_epi8(0,3,6,9,12,15,1,4,7,10,13,2,5,8,11,14);
	const __m128i smask1 = _mm_setr_epi8(6,7,8,9,10,0,1,2,3,4,5,11,12,13,14,15);
	const __m128i ssmask1 = _mm_setr_epi8(11,12,13,14,15,0,1,2,3,4,5,6,7,8,9,10);

	//GGGGGGBBBBBRRRRRへシャッフル
	const __m128i mask2 = _mm_setr_epi8(0,3,6,9,12,15, 2,5,8,11,14,1,4,7,10,13);
	//const __m128i smask2 = _mm_setr_epi8(6,7,8,9,10,0,1,2,3,4,5,11,12,13,14,15);smask1と同じ
	const __m128i ssmask2 = _mm_setr_epi8(0,1,2,3,4,11,12,13,14,15,5,6,7,8,9,10);

	//RRRRRRGGGGGBBBBBへシャッフル
	//__m128i mask3 = _mm_setr_epi8(0,3,6,9,12,15, 2,5,8,11,14,1,4,7,10,13);//mask2と同じ
	//const __m128i smask3 = _mm_setr_epi8(6,7,8,9,10,0,1,2,3,4,5,6,7,8,9,10);//smask1と同じ
	//const __m128i ssmask3 = _mm_setr_epi8(11,12,13,14,15,0,1,2,3,4,5,6,7,8,9,10);//ssmask1と同じ

	//ブレンドマスク
	const __m128i bmask1 = _mm_setr_epi8
		(255,255,255,255,255,255,0,0,0,0,0,0,0,0,0,0);

	const __m128i bmask2 = _mm_setr_epi8
		(255,255,255,255,255,255,255,255,255,255,255,0,0,0,0,0);

	const __m128i bmask3 = _mm_setr_epi8
		(255,255,255,255,255,0,0,0,0,0,0,0,0,0,0,0);

	const __m128i bmask4 = _mm_setr_epi8
		(255,255,255,255,255,255,255,255,255,255,0,0,0,0,0,0);	

	const int shift = 8;
	const int amp = 1<<shift;
	const int _R_=(int)(amp*0.299);
	const int _G_=(int)(amp*0.587);
	const int _B_=(int)(amp*0.114);
	const __m128i R = _mm_set1_epi16(_R_);
	const __m128i G = _mm_set1_epi16(_G_);
	const __m128i B = _mm_set1_epi16(_B_);
	const __m128i zero = _mm_setzero_si128();	

	for(int i=0;i<size;i+=48)
	{
		__m128i a = _mm_shuffle_epi8(_mm_load_si128((__m128i*)(s+i)),mask1);
		__m128i b = _mm_shuffle_epi8(_mm_load_si128((__m128i*)(s+i+16)),mask2);
		__m128i c = _mm_shuffle_epi8(_mm_load_si128((__m128i*)(s+i+32)),mask2);
		const __m128i aaaa = _mm_blendv_epi8(c,_mm_blendv_epi8(b,a,bmask1),bmask2);
		
		a = _mm_shuffle_epi8(a,smask1);
		b = _mm_shuffle_epi8(b,smask1);
		c = _mm_shuffle_epi8(c,smask1);
		const __m128i bbbb =_mm_blendv_epi8(c,_mm_blendv_epi8(b,a,bmask3),bmask2);

		a = _mm_shuffle_epi8(a,ssmask1);
		c = _mm_shuffle_epi8(c,ssmask1);
		b = _mm_shuffle_epi8(b,ssmask2);
		const __m128i cccc =_mm_blendv_epi8(c,_mm_blendv_epi8(b,a,bmask3),bmask4);

		__m128i a1 = _mm_unpackhi_epi8(aaaa,zero);
		__m128i a2 = _mm_unpacklo_epi8(aaaa,zero);
		 a1 = _mm_mullo_epi16(a1,B);
		 a2 = _mm_mullo_epi16(a2,B);

		 __m128i b1 = _mm_unpackhi_epi8(bbbb,zero);
		__m128i b2 = _mm_unpacklo_epi8(bbbb,zero);
		 b1 = _mm_mullo_epi16(b1,G);
		 b2 = _mm_mullo_epi16(b2,G);

		 __m128i c1 = _mm_unpackhi_epi8(cccc,zero);
		__m128i c2 = _mm_unpacklo_epi8(cccc,zero);
		 c1 = _mm_mullo_epi16(c1,R);
		 c2 = _mm_mullo_epi16(c2,R);

		 a1 = _mm_add_epi16(a1,b1);
		 a1 = _mm_add_epi16(a1,c1);
		 a2 = _mm_add_epi16(a2,b2);
		 a2 = _mm_add_epi16(a2,c2);

		 a1 = _mm_srli_epi16(a1,8);
		 a2 = _mm_srli_epi16(a2,8);
		 
		 a = _mm_packus_epi16(a1,a2);
		 
		_mm_stream_si128((__m128i*)(d),a);
		d+=16;
	}
}

void cvtBGR2GraySSE(Mat& src1,Mat& src2,Mat& src3, Mat& dest)
{
	const int size = src1.size().area();
	uchar* s1 = src1.ptr<uchar>(0);
	uchar* s2 = src2.ptr<uchar>(0);
	uchar* s3 = src3.ptr<uchar>(0);
	uchar* d = dest.ptr<uchar>(0);

	const int shift = 8;
	const int amp = 1<<shift;
	const int _R_=(int)(amp*0.299);
	const int _G_=(int)(amp*0.587);
	const int _B_=(int)(amp*0.114);

	const __m128i R = _mm_set1_epi16(_R_);
	const __m128i G = _mm_set1_epi16(_G_);
	const __m128i B = _mm_set1_epi16(_B_);
	
	const __m128i zero = _mm_setzero_si128();	

	for(int i=0;i<size;i+=16)
	{
		__m128i a = _mm_load_si128((__m128i*)(s1));
		__m128i b = _mm_load_si128((__m128i*)(s2));
		__m128i c = _mm_load_si128((__m128i*)(s3));
		
		__m128i a1 = _mm_unpackhi_epi8(a,zero);
		__m128i a2 = _mm_unpacklo_epi8(a,zero);
		 a1 = _mm_mullo_epi16(a1,B);
		 a2 = _mm_mullo_epi16(a2,B);

		 __m128i b1 = _mm_unpackhi_epi8(b,zero);
		__m128i b2 = _mm_unpacklo_epi8(b,zero);
		 b1 = _mm_mullo_epi16(b1,G);
		 b2 = _mm_mullo_epi16(b2,G);

		 __m128i c1 = _mm_unpackhi_epi8(c,zero);
		__m128i c2 = _mm_unpacklo_epi8(c,zero);
		 c1 = _mm_mullo_epi16(c1,R);
		 c2 = _mm_mullo_epi16(c2,R);

		 a1 = _mm_add_epi16(a1,b1);
		 a1 = _mm_add_epi16(a1,c1);
		 a2 = _mm_add_epi16(a2,b2);
		 a2 = _mm_add_epi16(a2,c2);

		 a1 = _mm_srli_epi16(a1,shift);
		 a2 = _mm_srli_epi16(a2,shift);
		 
		 a = _mm_packus_epi16(a1,a2);
		 
		_mm_stream_si128((__m128i*)(d+i),a);
		s1+=16;
		s2+=16;
		s3+=16;
	}
}

void cvtBGR2GrayBF(Mat& src, Mat& dest)
{
	const int size = src.size().area();
	uchar* s = src.ptr<uchar>(0);
	uchar* d = dest.ptr<uchar>(0);

	for(int i=0;i<size;i++)
	{
		d[0]=(uchar)(0.114*s[0]+0.587*s[1]+0.299*s[2]);
		s+=3;d++;
	}
}

void cvtBGR2GrayTab(Mat& src, Mat& dest, int shift=10)
{
	const int size = src.size().area();
	uchar* s = src.ptr<uchar>(0);
	uchar* d = dest.ptr<uchar>(0);
	
	const int amp = 1<<shift;
	const int R=(int)(amp*0.299);
	const int G=(int)(amp*0.587);
	const int B=(int)(amp*0.114);

	int* tab = new int[768];
	for(int i=0;i<256;i++)
	{
		tab[i]=B*i; tab[i+256]=G*i; tab[i+512]=R*i;
	}
	const int* b = tab;
	const int* g = tab+256;
	const int* r = tab+512;
	for(int i=0;i<size;i++)
	{
		d[0]=(b[s[0]]+g[s[1]]+r[s[2]])>>shift;
		s+=3;d++;
	}
	delete[] tab;
}
void cvtBGR2GrayInt(Mat& src, Mat& dest, const int shift=10)
{
	const int size = src.size().area();
	uchar* s = src.ptr<uchar>(0);
	uchar* d = dest.ptr<uchar>(0);

	const int amp = 1<<shift;
	const int R=(int)(amp*0.299);
	const int G=(int)(amp*0.587);
	const int B=(int)(amp*0.114);
	for(int i=0;i<size;i++)
	{
		d[0]=(B*s[0]+G*s[1]+R*s[2])>>shift;
		s+=3;d++;
	}
}

void vsOpenCV_BGR2Gray()
{
	const int iter = 10;
	
	Mat a(SIZE,CV_8UC3);
	a= CV_RGB(255,0,0);
	//Mat a = imread("lenna.png");

	vector<Mat> v(4);
	v[0].create(a.size(),a.type());
	v[1].create(a.size(),a.type());
	v[2].create(a.size(),a.type());
	v[3].create(a.size(),a.type());
	
	Mat b(a.size(),CV_8U);
	Mat b2(a.size(),CV_8U);
	
	int64 pre = getTickCount();
	for(int i=0;i<iter;i++)
		cvtColor(a,b2,CV_BGR2GRAY);
	cout<<"opencv time: "<<1000.0*(getTickCount()-pre)/(getTickFrequency())<<" ms"<<endl;

	//b.setTo(0);
	//pre = getTickCount();
	//for(int i=0;i<iter;i++)
	//	cvtBGR2GrayBF(a,b);
	//cout<<"normal time: "<<1000.0*(getTickCount()-pre)/(getTickFrequency())<<" ms"<<endl;
	//cout<<"L2 Norm:"<<cv::norm(b2,b)<<endl;

	//b.setTo(0);
	//pre = getTickCount();
	//for(int i=0;i<iter;i++)
	//	cvtBGR2GrayTab(a,b);
	//cout<<"table time: "<<1000.0*(getTickCount()-pre)/(getTickFrequency())<<" ms"<<endl;
	////cvtBGR2GraySSE(a,b);
	//cout<<"L2 Norm:"<<cv::norm(b2,b)<<endl;
	
	b.setTo(0);
	pre = getTickCount();
	for(int i=0;i<iter;i++)
		cvtBGR2GrayInt(a,b,8);
	cout<<"Int time: "<<1000.0*(getTickCount()-pre)/(getTickFrequency())<<" ms"<<endl;
	//cvtBGR2GraySSE(a,b);
	cout<<"L2 Norm:"<<cv::norm(b2,b)<<endl;

	b.setTo(0);
	pre = getTickCount();
	for(int i=0;i<iter;i++)
	{
		splitSSE__(a,v);
		cvtBGR2GraySSE(v[0],v[1],v[2], b);
	}
	cout<<"SSE time: "<<1000.0*(getTickCount()-pre)/(getTickFrequency())<<" ms"<<endl;
	cout<<"L2 Norm:"<<cv::norm(b2,b)<<endl;

	b.setTo(0);
	pre = getTickCount();
	for(int i=0;i<iter;i++)
	{
		cvtBGR2GraySSEShort(a, b);
	}
	cout<<"SSE time2: "<<1000.0*(getTickCount()-pre)/(getTickFrequency())<<" ms"<<endl;
	cout<<"L2 Norm:"<<cv::norm(b2,b)<<endl;
	
//	imshow("a",b);waitKey();
	
	/*
	b.setTo(0);
	pre = getTickCount();
	for(int i=0;i<iter;i++)
		cvtBGR2RGB_SSE_nonal(a,b);
	cout<<"SSE time: "<<1000.0*(getTickCount()-pre)/(getTickFrequency())<<" ms"<<endl;
	
	b.setTo(0);
	pre = getTickCount();
	for(int i=0;i<iter;i++)
		cvtBGR2RGB_SSE(a,b);
	cout<<"SSE time: "<<1000.0*(getTickCount()-pre)/(getTickFrequency())<<" ms"<<endl;
	*/
	//imshow("a",b);waitKey();	
}

void flip8UC1SSE(Mat& src, Mat& dest, int flipCode)
{
	if(flipCode==0)
	{
		const int size = src.cols/16;
		uchar* ss = src.data;
		uchar* dd = dest.data+(src.rows-1)*src.cols;
		for(int j=src.rows;j--;)
		{
			__m128i* s = (__m128i*)(ss);
			__m128i* d = (__m128i*)(dd);
			for(int i=size;i--;)
			{
				__m128i a = _mm_load_si128(s++);
				_mm_store_si128((d++),a);				
			}
			ss+=src.cols;
			dd-=src.cols;
		}
	}
	else if(flipCode==1)
	{
		const int size = src.cols/16;
		const __m128i mask = _mm_setr_epi8(15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0);
		uchar* ss = src.data + src.cols -16;
		uchar* dd = dest.data;
		for(int j=src.rows;j--;)
		{
			__m128i* s = (__m128i*)(ss);
			__m128i* d = (__m128i*)(dd);
			for(int i=size;i--;)
			{
				__m128i a = _mm_load_si128(s--);
				a = _mm_shuffle_epi8(a,mask);
				_mm_store_si128((d++),a);				
			}
			ss+=src.cols;
			dd+=src.cols;
		}
	}
	else if(flipCode==-1)
	{
		const __m128i mask = _mm_setr_epi8(15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0);
		const int size  =src.size().area()*src.channels();
		uchar* s = src.data+size-16;
		uchar* d = dest.data;
		for(int i=0;i<size;i+=16)
		{
			__m128i a = _mm_load_si128((__m128i*)(s));
			a = _mm_shuffle_epi8(a,mask);
			_mm_store_si128((__m128i*)(d+i),a);
			s-=16;
		}
	}
}

void flipSSE__(Mat& src, Mat& dest)
{
	const __m128i mask = _mm_setr_epi8(15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0);
	const int size  =src.size().area()*src.channels();
	uchar* s = src.data+size-16;
	uchar* d = dest.data;
	for(int i=0;i<size;i+=16)
	{
		__m128i a = _mm_load_si128((__m128i*)(s));
		a = _mm_shuffle_epi8(a,mask);
		_mm_store_si128((__m128i*)(d+i),a);
		s-=16;
	}
}
void flipBF(Mat& src, Mat& dest, int flipCode=1)
{
	if(flipCode==0)
	{
		for(int j=src.rows-1;j>=0;j--)
		{
			uchar* s = src.ptr<uchar>(j);
			uchar* d = dest.ptr<uchar>(src.rows-1-j);
			memcpy(d,s,src.cols);
		}
	}
	else if(flipCode==1)
	{
		for(int j=0;j<src.rows;j++)
		{
			uchar* s = src.ptr<uchar>(j);
			uchar* d = dest.ptr<uchar>(j);
			for(int i=0;i<src.cols;i++)
			{
				d[i] = s[src.cols-1-i];
			}
		}
	}
	else if(flipCode==-1)
	{
		uchar* s = src.data;
		uchar* d = dest.data;
		const int size  =src.size().area()*src.channels();
		for(int i=0;i<size;i++)
		{
			d[i] = s[size-1-i];
		}
	}
}

void vsOpenCV_flip()
{
	const int iter = 10;
	Mat a(SIZE,CV_8U);a= CV_RGB(255,0,0);
	//Mat a = imread("lenna.png",0);
	Mat b(a.size(),CV_8U);
	Mat b2(a.size(),CV_8U);
	
	for(int code = -1;code<2;code++)
	{
	int64 pre = getTickCount();
	for(int i=0;i<iter;i++)
		cv::flip(a,b,code);
	cout<<"opencv time: "<<1000.0*(getTickCount()-pre)/(getTickFrequency())<<" ms"<<endl;
	//imshow("b",b);waitKey();
	
	pre = getTickCount();
	for(int i=0;i<iter;i++)
		flipBF(a,b,code);
	cout<<"BF time: "<<1000.0*(getTickCount()-pre)/(getTickFrequency())<<" ms"<<endl;
	//imshow("b",b);waitKey();

	pre = getTickCount();
	for(int i=0;i<iter;i++)
		flip8UC1SSE(a,b,code);
	cout<<"SSE time: "<<1000.0*(getTickCount()-pre)/(getTickFrequency())<<" ms"<<endl;
	//imshow("b",b);waitKey();
	}
	
}