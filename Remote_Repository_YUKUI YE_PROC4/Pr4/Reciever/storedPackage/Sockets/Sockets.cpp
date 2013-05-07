xmlMetaData*yye*1*a.h$Sockets.h/////////////////////////////////////////////////////////////////////
// Sockets.cpp - Provides basic network communication services     //
// ver 3.1                                                         //
// Language:      Visual C++, 2005                                 //
// Platform:      Dell Dimension 9150, Windows XP Pro, SP 2.0      //
// Application:   Utility for CSE687 and CSE775 projects           //
// Author:        Jim Fawcett, Syracuse University, CST 2-187      //
//                (315) 443-3948, jfawcett@twcny.rr.com            //
/////////////////////////////////////////////////////////////////////

#include "Sockets.h"
#include "../threads/locks.h"
#include <sstream>

#ifdef TRACING
  #include "..\threads\locks.h"
  #define TRACE(msg) sout << "\n  " << msg;
#else
  #define TRACE(msg) ;
#endif

long SocketSystem::count = 0;

//----< convert integer to string >----------------------------------

std::string IntToString(const int num)
{
  std::ostringstream out;
  out << num;
  return ou
  DWORD dwMessageID = errorCode;
  DWORD dwLanguageId = MAKELANGID(LANG_ENGLISH,SUBLANG_ENGLISH_US);
  LPSTR lpBuffer;
  DWORD nSize = 0;
  va_list *Arguments = NULL;

  FormatMessage(
    dwFlags,lpSource,dwMessageID,dwLanguageId, 
    (LPTSTR)&lpBuffer,nSize,Arguments
  );

  std::string _msg(lpBuffer);
  LocalFree(lpBuffer);
  return _msg;
}
//
//----< load WinSock Library >---------------------------------------

SocketSystem::SocketSystem()
{
  if(count == 0)
  {
    TRACE("loading wsock32 library");
 ption("initialization error: ");
  }
  InterlockedIncrement(&count);
}
//----< destructor unloads socket library >--------------------------

SocketSystem::~SocketSystem()
{ 
  try {
    if(InterlockedDecrement(&count) == 0)
    {
      TRACE("unloading wsock32 library");
      WSACleanup();
    }
  }
  catch(...) { /* don't allow exception to propagate on shutdown */}
}
//----< get host name >----------------------------------------------

std::string SocketSystem::getHostName()
{
  char buffer[256];
  gethostname(buffer,256);
  return buffer;
}
//
//----< get ip address of network machine >--------------------------

std::string SocketSystem::getIpFromName(const std::string& name)
{
  hostent* remoteHost = 0;
  SOCKADDR_IN tcpAddr;
  tcpAddr.sin_family = AF_IAF_It_addr(ip.c_str());
  host = gethostbyaddr((char*)ipaddr, sizeof(ipaddr), AF_INET);
  if(!host)
    throw std::exception("name resolution error: ");    
  return host->h_name;
}
//----< constructor creates TCP Stream socket >----------------------

Socket::Socke-< promotes WinSock SOCKET handle to Socket object >------------

Socket::Socket(SOCKET s) : s_(s) {}

//----< destructor closes socket handle >----------------------------

Socket::~Socket()
{ 
  TRACE("destroying socket");
  closesocket(s_);
  //disconnect();
}
//----< assignment >-------------------------------------------------

Socket& Socket::operator=(const Socket& sock)
{
  if(this == &sock) return *this;
  TRACE("copying socket");
  DuplicateHandle(GetCurrentProcess(),(HANDLE)sock.s_,GetCurrentProcess(),(H connects to IP address or network host >---------------------

bool Socket::connect(std::string url, int port, bool throwError, size_t MaxTries)
{
  try {
    if(isalpha(url[0]))
      url = SocketSystem().getIpFromName(url);
  }
  catch(...)
  {
    if(throwError)
      throw std::exception(ss_.GetLastMsg(true).c_str());
    return false;
  }
  SOCKADDR_IN tcpAddr;
  tcpAddr.sin_family = AF_INET;
  tcpAddr.sin_addr.s_addr = inet_addr(url.c_str());
  tcpAddr.sin_port = htons(port);
  if(s_ == INVALID_SOCKET)
  {
    s_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  }
  size_t tryCount = 0;
  std::string countStr;
  while(true)
  {
    ++tryCount;
    TRACE("attempt to connect #" + IntToString(tryCount));
    int err = ::connect(s_, (sockaddr*)&tcpAddr, sizeof(tcpAddrof(tcpAddr--------------

int Socket::send(const char* block, size_t len)
{
  return ::send(s_,block,len,0);
}
//----< recieve byte block >-----------------------------------------

int Socket::recv(char* block, size_t len)
{
  return ::recv(s_,block,len,0);
}
//----< return nu0;
  size_t blockLen = len;
  size_t bytesLeft = blockLen;
  while(bytesLeft > 0) {
    bytesSent = ::send(s_,&block[blockIndx],static_cast<int>(bytesLeft),0);
    if(bytesSent == SOCKET_ERROR)
    {
      sout << "\n  socket error";
      ++count;
      Sleep(50);
      bytesSent = 0;
    }
    //sout << "\n  sending retry";
    if(bytesSent == WSAECONNRESET)
    {
      sout << "\n  connection broken";
      if(throwError)
        throw std::exception("connection closed");
      return false;
    }
    if(count==sendReIndx += bytesSent;
  }
  return true;
}
//----< blocks until len characters have been sent >-----------------

bool Socket::recvAll(char* block, size_t len, bool throwError)
{
  const size_t recvRetries = 100;
  size_t bytesRecvd, bytesLeft = len;
  size_t blockIndx = 0, count = 0;
  while(bytesLeft > 0) {
    bytesRecvd = ::recv(s_,&block[blockIndx],static_cast<int>(bytesLeft),0);
    if(bytesRecvd == 0)
    {
      if(throwError)
        throw(std::exception("remote connection closed"));
      return false;
    }
    if(bytesRecvd == SOCKET_ERROR) {
      ++count;
      Sleep(50);
    }
    if(bytesRecvd == WSAECONNRESET)
    {
      if(throwError)
        throw(std::exception("connection closed"));
      return false;
    }
    if(count==recvRetries)
    {
      if(throw{
      if(throweads to end of buffer
 * - returns empty string if not successful
 *
 */
std::string Socket::readLine()
{
  std::string temp;
  char block[1];
  //while(bytesLeft() > 0)  // don't block
  while(true)
  {
    if(!recvAll(block,1))
      return "";
    // save all chars that tLocalIP()
{
  //struct sockaddr name;
  //int len = sizeof(name);
  hostent* local = gethostbyname("");
  return inet_ntoa(*(struct in_addr*)*local->h_addr_list);
}
//----< get local port >---------------------------------------------

int SocketSystem::getLocalPort(Socket* pSock)
{
  struct sockaddr name;
  int len = sizeof(name);
  int status = getsockname(*pSock,&name,&len);
  if(status == 0)
  {
    struct sockaddr_in* pLocal = reinterpret_cast<sockaddr_in*>(&name);
    return htons(pLocal->sin_port);
  }
  return -1;
}
/== 0)
  {
    struct sockaddr_in* pRemote = reinterpret_cast<sockaddr_in*>(&name);
    return inet_ntoa(pRemote->sin_addr);
  }
  return "";
}
//----< get remote port >--------------------------------------------

int SocketSystem::getRemotePort(Socket* pSock)
{
  struct sockaddr name;
  int len = sizeof(name);
  int status = getpeername(*pSock,&name,&len);
  if(status == 0)
  {
    struct sockaddr_in* pRemote = reinterpret_cast<sockaddr_in*>(&name);
    return htons(pRemote->sin_port);
  }
  return -1;
}
//
//----< starts listener socket listening for connections >-----------

SocketListener::SocketListener(int port) : InvalidSocketCount(0)
{
  tcpAddr.sin_family = AF_INET;   // TCP/IP
  tcpAddr.sin_port = htons(port); // listening port
  tcpAddr.sin_addr.s_addr = INADDR_ANY;
 s_addr = INADDR_ANY;
 until a connection request has been received >--------

SOCKET SocketListener::waitForConnect()
{
  const long MaxCount = 20;
  InvalidSocketCount = 0;
  TRACE("listener waiting for connection request");
  int size = sizeof(tcpAddr);
  SOCKET toClient;
  do {
    toClient = accep-----------------------------------

#ifdef TEST_SOCKETS
#include <iostream>

void main()
{
  /*
   * Note: 
   * - No threads are used in this test stub, so it doesn't represent typical usage.
   * - Neither do we do any message framing.  So correct operation depends on lightly
   *   loaded receiver.
   * - We will provide a simple demo using threads and queues with message framing
   *   to show how that is done in another demo.  
   */
  std::cout << "\n  Testing Socket, SocketListener, and SocketSystem classes";
  std::cout << = su.getIpFromName(host);
    std::cout << "\n  IP Address of machine:       " << ip.c_str();
    std::string name = su.getNameFromIp(ip);
    std::cout << "\n  DNS name of machine from ip: " << name.c_str() << '\n';
  
    std::cout << "\n  Client connecting to Server";
    std::cout << "\n -----------------------------";
    SocketListener listener(2048);
    Socket sendr;
    if(!sendr.connect("127.0.0.1",2048))  // this works too
    //if(!sendr.connect("Apocalypse",2048))  // can use ip addr, e.g., 127.0.0.1
    {
      std::cout << "\n connection failed\n\n";
      return;
    }
    Socket recvr = listener.waitForConnect();
    std::cout << "\n  remote ip is: " << recvr.System().getRemoteIP(&recvr);
    std::cout << ", remote port is: " << recvr.System().getRemotePort(&recvr);
  ().getRemotePort(&recvr);
  eceived: " << buffer;
    std::cout << "\n  " << recvr.bytesLeft() << " bytes ready for reading";

    recvr.recvAll(buffer,strlen(msg2)+1);
    std::cout << "\n  Server received: " << buffer;
    std::cout << "\n  " << recvr.bytesLeft() << " bytes ready for reading";

    recvr.recvAlForConnect();
    msg1 = "another message after reconnecting";
    std::cout << "\n  Client sending: " << msg1;
    sendr.WriteLine(msg1);
    std::string temp = recvr.ReadLine();
    std::cout << "\n  Server received: " << temp;
    std::cout << "\n";

    // demonstrating full duplex operation
    std::cout << "\n  sending from Server back to Client";
    std::cout << "\n ------------------------------------";

    // sending
    msg1 = "sending message back";
    std::cout << "\n  Server sending message: " << msg1;
    recvr.WriteLine(dr.ReadLine();
    std::cout << "\n  Client received: " << sendr.ReadLine();
    std::cout << std::endl;

    // copy construction
    Socket sendrCopy = sendr;
    Socket recvrCopy = recvr;

    // copy construction
    std::cout << "\n  sending and recieving with socket copies";
    std::cout << "\n ------------------------------------------";

    sendrCopy.WriteLine("string from sendrCopy");
    // recieving with copy
    std::cout << "\n  recvrCopy received: " << recvrCopy.ReadLine();
    std::cout << std::endl;

    // socket assignment
    sendr = sendrCopy;
    recvr = recvrCopy;

    // sending from assigned socket
    std::cout << "\n  sending and recieving with assigned sockets";
    std::cout << "\n ---------------------------------------------";

    sendr.WriteLine("string from

    sendr.WriteLine("string from