#include "SecondTestfile.h"
#include <sstream>

class mm2
{
};
class classFromSecondtestfile
{
	B b;  //package composition between firstTestfile and secondTestfile
	void function1FromSecondTestfile
	{
		//function1FromSecondTestFile calls a global function defined in firstTestFile
		gf();	
	}
	void function2FromSecondTestfile
	{
		std::cout<<"\nfunction2FromSecondTestFile assign globalValue defined in FirstTestfile to secondTestfile instance\n";
		std::string str = gf2();
		std::cout<<"valueFromSecondTestFile:\n"<<valueFromSecondTestFile;
	}
};