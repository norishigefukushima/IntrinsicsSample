#include "function.h"

void printm128i(__m128i in, int num)
{	
	if(num==16)printf("%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n",in.m128i_i8[0],in.m128i_i8[1],in.m128i_i8[2],in.m128i_i8[3],in.m128i_i8[4],in.m128i_i8[5],in.m128i_i8[6],in.m128i_i8[7],in.m128i_i8[8],in.m128i_i8[9],in.m128i_i8[10],in.m128i_i8[11],in.m128i_i8[12],in.m128i_i8[13],in.m128i_i8[14],in.m128i_i8[15]);
	if(num==8)printf("%d %d %d %d %d %d %d %d\n",in.m128i_i16[0],in.m128i_i16[1],in.m128i_i16[2],in.m128i_i16[3],in.m128i_i16[4],in.m128i_i16[5],in.m128i_i16[6],in.m128i_i16[7]);
	if(num==4)printf("%d %d %d %d\n",in.m128i_i32[0],in.m128i_i32[1],in.m128i_i32[2],in.m128i_i32[3]);
	if(num==2)printf("%d %d\n",in.m128i_i64[0],in.m128i_i64[1]);
}

void checkCPU(void)
{
	int CPUInfo[4];
	int InfoType = 1;
	__cpuid(CPUInfo, InfoType);

	if (CPUInfo[3] & 0x00800000) printf("support MMX\n");
	else printf("DO NOT support MMX\n");
	if (CPUInfo[3] & 0x02000000) printf("support SSE\n");
	else printf("DO NOT support SSE\n");
	if (CPUInfo[3] & 0x04000000) printf("support SSE2\n");
	else printf("DO NOT support SSE2\n");
	if (CPUInfo[2] & 0x00000001) printf("support SSE3\n");
	else printf("DO NOT support SSE3\n");
	if (CPUInfo[2] & 0x00080000) printf("support SSE4.1\n");
	else printf("DO NOTsupport SSE4.1\n");
	if (CPUInfo[2] & 0x00100000) printf("support SSE4.2\n");
	else printf("DO NOT support SSE4.2\n");
	return ;
}