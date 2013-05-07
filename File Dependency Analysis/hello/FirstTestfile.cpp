#include "FirstTestfile.h"
#include <sstream>

class A
{
};
class B
{
};

void gf()
{
	std::cout<<"\n Hello I am a global FunctionFromFirstTestFile\n";
}

std::string gf2()
{
	return "HelloHello how are you :)";
}

/*class B:public A
{
};*/
/*class C
{
};
class D
{
	C c;
};*/

class E
{
	void m()
	{
		G g=new G();
	}
}
class W
{
};
/*class H
{
	void m(W & w)
	{
	};
};*/

struct structClasselement
{
  std::string type;
  std::string name;

};
enum enumClassrelation { inheritance, composition};
//typedef element simple;
