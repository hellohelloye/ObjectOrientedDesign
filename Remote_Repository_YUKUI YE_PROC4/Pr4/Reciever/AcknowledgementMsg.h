/////////////////////////////////////////////////////////////////////
//AcknowledgementMsg : structure                                   //
// Language:    Visual C++ 2012                                    //
// Platform:    MACPRO, Virtual Machine Windows 8                  //
// Application: Help for CSE687 - OOD, Pr#1, Spring 2013           //
// Author:      YUKUI YE,            Syracuse University           //
//              (315) 751-0156, yye@syr.edu                        //
/////////////////////////////////////////////////////////////////////
#ifndef ACKNOWLEDGEMENTMSG_H
#define ACKNOWLEDGEMENTMSG_H
#include "../Sockets/Sockets.h"
#include "../Threads/Threads.h"
#include "../Threads/Locks.h"
#include <string>
#include <iostream>
#include <sstream>


struct AcknowlegementMsg
{
	std::string IP;
	int srcPort;
	std::string type;
	std::string msg;
	std::string fileName;
	std::string retmsg;
};
#endif