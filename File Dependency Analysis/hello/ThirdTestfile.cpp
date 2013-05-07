#include "ThirdTestfile.h"
#include <sstream>

class classFromThirdTestFile: public A  //package Inheritance between firstTestfile and ThirdTestfile
{
	void functionFromThirdTestfile( W & w)
	{
		std::cout<<"\nPackage Using,between class W from firstTestfile and classFromThirdTestFile\n";
	}
};

class G
{
	std::cout<<"firstTestfile would have aggregation with thirdTestfile\n";
};