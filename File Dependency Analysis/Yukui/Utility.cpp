// MTutility.cpp/////////////////////////////////////////////////////
//  Language:       Visual C++, ver 6.0                            //
//  Platform:       MacBook Pro, Windows7 Pro				       //
//  Application:    CSE687 project #3	                           //
//  Author:         YUKUI YE, Syracuse University	     		   //
//                  (315) 751-0156, mollysmile.ye@gmail.com        //
/////////////////////////////////////////////////////////////////////
#include <iostream>
#include <string>

void Title(const std::string& str, char underline='=', std::ostream& out=std::cout)
{
  out << "\n  " << str;
  out << "\n " << std::string(str.size() + 2, underline);
}