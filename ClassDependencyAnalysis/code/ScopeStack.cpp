///////////////////////////////////////////////////////////////////////////////
// ScopeStack.cpp - implements template stack holding specified element type //
// ver 2.0                                                                   //
//  Language:       Visual C++, ver 6.0                                      //
//  Platform:       MacBook Pro, Windows7 Pro				                 //
//  Application:    CSE687 project #3	                                     //
//  Author:         YUKUI YE, Syracuse University	     		             //
//                  (315) 751-0156, mollysmile.ye@gmail.com                  //
///////////////////////////////////////////////////////////////////////////////

//----< test stub >------------------------------------------------------

#ifdef TEST_SCOPESTACK

#include "ScopeStack.h"
#include <string>
#include <iostream>
#include <sstream>

struct element
{
  std::string type;
  std::string name;
  size_t lineCount;
  std::string show()
  {
    std::ostringstream temp;
    temp << "(";
    temp << type;
    temp << ", ";
    temp << name;
    temp << ", ";
    temp << lineCount;
    temp << ")"; 
    return temp.str();
  }
};

typedef ScopeStack<element> stack;

int main()
{
  std::cout << "\n  Testing Scope Stack";
  std::cout << "\n =====================\n";

  std::cout << "\n  pushing items onto ScopeStack instance";
  std::cout << "\n ----------------------------------------";

  stack testStack;
  element item;

  item.type = "function";
  item.name = "fun1";
  item.lineCount = 33;
  testStack.push(item);

  item.type = "function";
  item.name = "fun2";
  item.lineCount = 53;
  testStack.push(item);

  item.type = "function";
  item.name = "fun3";
  item.lineCount = 73;
  testStack.push(item);


  showStack<element>(testStack, false);
  std::cout << std::endl;

  std::cout << "\n  Popping two items off ScopeStack";
  std::cout << "\n ----------------------------------";

  testStack.pop();
  testStack.pop();

  showStack<element>(testStack, false);
  std::cout << "\n\n";
}

#endif
