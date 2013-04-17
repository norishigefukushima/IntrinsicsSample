#include "function.h"
#include <nmmintrin.h> //MMX - SSE4.2 
#include <tmmintrin.h>
#include <smmintrin.h>
#include <intrin.h>      // MMX-SSE3命令セットを使用する場合インクルード   //
#include <emmintrin.h>   // MMX-SSE2命令セットを使用する場合インクルード   //
#include <xmmintrin.h>   // MMX-SSE命令セットを使用する場合インクルード    //
#include <mmintrin.h>    // MMX命令セットを使用する場合インクルード        //


int main(int argc, char** argv)
{
	setUseOptimized(true);
	
	checkCPU();
	typecvtS2L();//型の変換
	typecvtL2S();
	addmullsingle();
	shuffle_test();

	
	cout<<"copy test"<<endl;vsOpenCV_copy();
	cout<<"cvt test"<<endl;vsOpenCV_cvt();
	cout<<"add test"<<endl;vsOpenCV_add();
	cout<<"multiply test"<<endl;vsOpenCV_multiply();
	cout<<"flip test"<<endl;vsOpenCV_flip();
	cout<<"swap test"<<endl;vsOpenCV_swapRB();
	cout<<"split test"<<endl;vsOpenCV_split();
	cout<<"gray test"<<endl;vsOpenCV_BGR2Gray();
	
    return 0;
}