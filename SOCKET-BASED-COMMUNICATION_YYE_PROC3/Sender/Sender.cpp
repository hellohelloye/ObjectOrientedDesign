/////////////////////////////////////////////////////////////////////
// Sender.cpp - Demonstration of concurrent socket connectors      //
// Language:    Visual C++ 2012                                    //
// Platform:    MACPRO, Virtual Machine Windows 8                  //
// Application: Help for CSE687 - OOD, Pr#1, Spring 2013           //
// Author:      YUKUI YE,            Syracuse University           //
//              (315) 751-0156, yye@syr.edu                        //
/////////////////////////////////////////////////////////////////////
/*
 * This Sender expects to write lines of text only.
 * So message framing is done by lines.  
 *
 * For HTTP like protocols the Sender should send lines for each
 * header attribute and bytes in the body, if there is one,
 * specified by a last header line something like:
 *    content_length : 1024
 * where 1024 is a stand-in for whatever you want your block 
 * size to be.
 *
 */
/*
 * Required files:
 * - Sender.cpp, Sockets.h, Sockets.cpp, 
 *   Threads.h, Threads.cpp, Locks.h, Locks.cpp
 *   BlockingQueue.h, BlockingQueue.cpp
 *
 * Maintanence History:
 * ver 1.1 - 30 Mar 2013
 * - changed Sendthread from terminating to default
 * - minor changes to error handling
 * ver 1.0 - 29 Mar 2013
 * - first release
 */

#include "../Sender/Sender.h"


/////////////////////////////////////////////
//int choose:"I messageType: 1:SendText; 2:SendFile"
//std::string inputInfor : textMessage or path
//int souceport
//int style : if inputInfor is the folder path then style=1;
//            if inputInfor is specificPath then style=2;
//int remotePort
//std::string IP
//////////////////////////////
int main(int argc, char* argv[])
{
    int choose=1,srcPortNum,style;
	std::string inputInfor;
	choose =atol( argv[1]);
	inputInfor = argv[2];
	srcPortNum = atol(argv[3]);
	style = atol(argv[4]);
	int remotePort = atol(argv[5]);
	std::string IP = argv[6];
	
	bool test = getInforFromCommand(choose,inputInfor,srcPortNum,style,remotePort,IP);
}
