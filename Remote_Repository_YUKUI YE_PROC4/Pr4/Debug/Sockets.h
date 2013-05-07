#ifndef SOCKETS_H
#define SOCKETS_H
/////////////////////////////////////////////////////////////////////
// Sockets.h   -  Provides basic network communication services    //
// ver 3.1                                                         //
// Language:      Visual C++, 2005                                 //
// Platform:      Dell Dimension 9150, Windows XP Pro, SP 2.0      //
// Application:   Utility for CSE687 and CSE775 projects           //
// Author:        Jim Fawcett, Syracuse University, CST 2-187      //
//                (315) 443-3948, jfawcett@twcny.rr.com            //
/////////////////////////////////////////////////////////////////////
/*
   Module Operations:
   ==================
   This module provides network communication services, using 
   WinSock2, a nearly Berkley Sockets compliant implementation.
   Three classes are provided:

   SocketSystem:
   -------------
   provides WinSock loading, unloading and a few program wide services.
   A recent change has ensured that the WinSocener
   Socket recvr = listener.waitForConnect();  // start listener listening
   Socket sendr;                              // create sending socket
   sender.connect("\\localhost",2048);        // request a connection
   const char* msg = "this is a message"; 
   sender.sendAll(msg,strlen(msg)+1);         // send msg and terminating null
   sender.sendAll("quit",strlen("quit")+1);   // send another msg

   char* buffer[1024];                        // receive buffer
   recvr.recvAll(buffer,strlen(msg)+1);       
   std::string reply = recvr.ReadLine();      // removes newline

   recvr.disconnect();                        // graceful shutdown
   sender.disconnect();                       // graceful shutdown
*/
//
/*
   Build Process:
   ==============
   Required Files:
     Sockets.h, Sockets.cpp

   Compile Command:
   ================
   cl /EHsc /DTEST_SOCKETS Sockets.cpp wsock32.lib user32.lib

   Maintenance History:
   ====================
   ver 3.1 : 29 Mar 2013
   - changed ReadLine to readLine   -- breaking change
   - changed WriteLine to writeLine -- breaking change
   - removed a check for bytes > 0 in ReadLine()
   - modified error handling in sendAll
   ver 3.0 : 15 Apr 2012
   - changed send to sendAll -- breaking change
   - changed recv to rec rec Apr 08
   - discovered that WinSock connect function does not always return 
     appropriate error message on failure to connect (this is a new
     bug in WinSock, I believe) so added a remote port check, which
     returns -1 on failure to connect.
   ver 1. some commented code
   ver 1.3 : 27 Mar 07
   - added SocketSystem members to Socket and SocketListener
   ver 1.2 : 18 Feb 07
   - cosmetic changes
   ver 1.0 : 11 Feb 07
   - first release, a major rewrite of an earlier socket demo

   Planned Changes:
   ================
   - Add reference counting by duplicating socket handle and change access
     of copy and assignment to public.

*/

#include <string>
#include <winsock2.h>

/////////////////////////////////////////////////////////////////////
// SocketSystetd::string getIpFromName(const std::string& name);
  std::string getRemoteIP(Socket* pSock);
  int getRemotePort(Socket* pSock);
  std::string getLocalIP();
  int getLocalPort(Socket* pSock);
  std::string GetLastMsg(bool WantSocketMsg=true);
private:
  static long count;
};

/////////////////////////////////////////////////////////////////////
// Socket class provides basic connect, read, and write operations

class Socket
{
public:
  Socket();
  Socket(const Socket& sock);
  Socket(SOCKET s);
  ~Socket();
  Socket& operator=(const Socket& sock);
  Socket& operator=(SOCKET sock);
  operator SOCKET ();
  bool connect(std::string url, int port, bool throwError=false, size_t MaxTries=50);
  void disconnect();
  bool error() { return (s_ == SOCKET_ERROR); }
  int send(cont send(co
  Socket s_;
  SocketSystem ss_;
  volatile long InvalidSocketCount;
};

inline long SocketListener::getInvalidSocketCount() 
{ 
  return InvalidSocketCount; 
}

#endif
ÿ