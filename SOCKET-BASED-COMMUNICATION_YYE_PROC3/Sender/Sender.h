/////////////////////////////////////////////////////////////////////
// Sender.cpp - Demonstration of concurrent socket connectors      //
// Language:    Visual C++ 2012                                    //
// Platform:    MACPRO, Virtual Machine Windows 8                  //
// Application: Help for CSE687 - OOD, Pr#1, Spring 2013           //
// Author:      YUKUI YE,            Syracuse University           //
//              (315) 751-0156, yye@syr.edu                        //
/////////////////////////////////////////////////////////////////////
#ifndef SENDER_H
#define SENDER_H
#include "../sockets/Sockets.h"
#include "../Threads/Threads.h"
#include "../Threads/Locks.h"
#include "../BlockingQueue/BlockingQueue.h"
#include <string>
#include <iostream>
#include <sstream>
#include "../Sender/FileManager.h"
#include "../Base64/Base64.h"


///////////////////////////////////////////////////
// SendThread thread

class SendThread : public threadBase
{
public:
  SendThread(Socket s, BlockingQueue<std::string>& q) : s_(s), q_(q) {}
  std::string& status() { return status_; }
private:
  void run()
  {
    status_ = "good";
    doLog("send thread running");
    std::string msg;
    do
    {
      doLog("send thread enqing msg");
      msg = q_.deQ();
      if(!s_.writeLine(msg))
      {
        sout << "\n  bad status in sending thread";
        status_ = "bad";
        break;
      }
    } while(msg != "stop");
    s_.disconnect();
  }
  std::string status_;
  Socket s_;
  BlockingQueue<std::string>& q_;
};

std::string ToString(int i)
{
  std::ostringstream conv;
  conv << i;
  return conv.str();
}

//chunk the file into block based on blockSize
std::string chunkFile(std::string tp,std::string specificPath)
{
	std::fstream in(specificPath);
	std::string msg;
	const static int blockSize=1024;
	while(in.good())
	{
		std::string block;
		for(size_t i=0;i<blockSize;i++)
		{
			if(!in.good()) break;
			block += in.get();
		} 
		std::vector<char> vmsg(block.begin(), block.end());
		std::string temp = Base64::encode(vmsg);
		msg += temp ;	
	}
	return msg;
}
std::string chunkFile2(std::string tp,std::string specificPath)
{
	std::fstream in(specificPath);
	std::string msg;
	const static int blockSize=1024;
	while(in.good())
	{
		std::string block;
		for(size_t i=0;i<blockSize;i++)
		{
			if(!in.good()) break;
			block += in.get();
		}	
		msg +=block;
	}
	return msg;
}

class Sender
{
public:
  Sender(int numMsgs,std::string inputmsg,int choose) : numMsgs_(numMsgs),inputInfor(inputmsg),fileType(choose) { myCount = ++count; }
  int id() { return myCount; }
  void start(std::string ip, int destPort, int sourcePort) {
    sout << locker << "\n Sender #" << id() << " started" << unlocker;
    pSt = new SendThread(s_, q_);
    pSt->start();
    if(!s_.connect(ip, destPort)) {
      sout << locker << "\n  couldn't connect to " << ip << ":" << destPort << "\n\n" << unlocker;
      delete pSt;
      return;
    } else{std::string logMsg = "\n  connected to " + ip + ":" + ToString(destPort);
      doLog(logMsg.c_str());
    }doLog("starting Sender");
    std::string msg;
	if(fileType==1){
      for(int i=0; i<numMsgs_; ++i) {
      doLog("sending message");
	  std::string temp = "<msg>"; 
      temp += std::string("<IP>") + ip + "</IP>"; //I assume the sourceIP is the same as ReceiverIPaddress
	  temp += std::string("<sourcePort>") + ToString(sourcePort) + "</sourcePort>";
	  temp += std::string("<type>") + "text" + "</type>";
      temp += std::string("<body>") + inputInfor + "</body></msg>";
      sout << locker << "\n  " << temp.c_str() << unlocker;
      q_.enQ(temp);
      ::Sleep(10 * id());  // sleep time increases with each addition Sender
      if(pSt->status() == "bad")
        break;}
	}
	if(fileType==2){
		for(int i=0; i<numMsgs_; ++i) {
      doLog("sending message");
	  std::string temp = "<msg>";
      temp += std::string("<IP>") + ip + "</IP>"; //I assume the sourceIP is the same as ReceiverIPaddress
	  temp += std::string("<sourcePort>") + ToString(sourcePort) + "</sourcePort>";
      temp += std::string("<type>") + "file" + "</type>";
	  int pos = inputInfor.find_last_of("/");
	  std::string filename = inputInfor.substr(pos+1);
      temp += std::string("<fileName>") + filename + "</fileName></msg>";
	  std::string temp1 = temp + std::string("<body>") + chunkFile(temp,inputInfor) + "</body>";
	  std::string temp2 = temp + std::string("<body>")+ chunkFile2(temp,inputInfor)+"</body>";
	  sout << locker << "\n  " << temp2.c_str() << unlocker;
	  q_.enQ(temp1);
      ::Sleep(10 * id());  // sleep time increases with each addition Sender
      if(pSt->status() == "bad")
        break;}
	} q_.enQ("stop");
    msg = "sender#" + ToString(id()) + ": stop";
    sout << "\n  " + msg;
    pSt->join();
    delete pSt;
  }
private:
  Socket s_;
  BlockingQueue<std::string> q_;
  SendThread* pSt;
  static int count;
  int myCount;
  int numMsgs_;
  std::string inputInfor;
  int fileType;
};

int Sender::count = 0;

///////////////////////////////////////////////////
// DemoThread is used to get two or more senders
// running concurrently from a single process, to
// make testing easier.

class DemoThread : public threadBase
{
public:
	DemoThread();
  DemoThread(Sender sndr,std::string ip,int destinationPort,int sourcePort) : sndr_(sndr),
  _IP(ip),_destinationPort(destinationPort),_sourcePort(sourcePort){}

private:
  void run()
  {
    
    sndr_.start(_IP, _destinationPort,_sourcePort);
  }
  Sender sndr_;
  int _destinationPort;
  int _sourcePort;
  std::string _IP;
};
///////////////////////////////////////////////////
// ClientHandlerThread thread

class ClientHandlerThread : public tthreadBase
{
public:
  ClientHandlerThread(Socket s, BlockingQueue<std::string>& q) : s_(s), q_(q) {}
private:
  void run()
  {
    std::string msg;
    do
    {
      doLog("receive thread reading line");
      msg = s_.readLine();
      if(msg == "")
        break;
      q_.enQ(msg);
    } while(msg != "quit");
  }
  Socket s_;
  BlockingQueue<std::string>& q_;
};

///////////////////////////////////////////////////
// listenThread thread
class ListenThread : public threadBase
{
public:
  ListenThread(int port, BlockingQueue<std::string>& q) : listener_(port), q_(q), stop_(false) {}
  void stop(bool end) { stop_ = end; }
private:
  void run()
  {
    while(!stop_)
    {
      SOCKET s = listener_.waitForConnect();
      ClientHandlerThread* pCh = new ClientHandlerThread(s, q_);
      pCh->start();
    }
  }
  bool stop_;
  BlockingQueue<std::string>& q_;
  SocketListener listener_;
};

class Receiver
{
protected : bool state;
public:
	bool getState()
	{
		return state;
	}
  void start(int port)
  {
    sout << "\nReciever started\n";
    pLt = new ListenThread(port, q_);
    try
    {
      pLt->start();
      std::string msg;
      int count = 0;
      do
      {
        msg = q_.deQ();
		if(msg.find("successfully")== std::string::npos || msg.find("permitted")==std::string::npos) 
			state = true;
		else if(msg.find("denied") == std::string::npos) state = false;
		std::cout << "\n"<<state;
        doLog((std::string("fetching message ") + msg).c_str());
        sout << "\n  Recieved from: " << msg.c_str();
      } while(msg != "stop");
    } catch(std::exception& ex){delete pLt;}
    catch(...){ delete pLt; }
  }
private:
  BlockingQueue<std::string> q_;
  ListenThread* pLt;
};
//class myChannel
//{
//public:
//global function to get information from user and send the relative file
bool getInforFromCommand(int choose,std::string infor,int port,int st,int rmtPort,std::string ip)
{
  int srcPortNum = port, remotPort = rmtPort; int ret = 0;
  std::string inputInfor = infor,IP = ip;
 if(choose==1){
	  try{Sender sdr(1,inputInfor,choose);
	  DemoThread t1(sdr,IP,remotPort,srcPortNum);
	  t1.start(); t1.join();
	  Receiver rcvr; rcvr.start(srcPortNum); 
	  return rcvr.getState();
	  }
	  catch(std::exception& ex){  sout << "\n\n  " << ex.what();ret = 1; }
	  catch(...) {sout << "\n\n  something bad happend to a sender"; ret = 1; }
	  std::cout << "\n\n  That's all Folks!\n\n";
 }

 if(choose==2){
	 FileManager fm; int style = st;
	 if(style == 1){
	 std::vector<std::string> files = fm.getFileList(inputInfor);
	 for(size_t i=0;i<files.size();i++){   
		 std::string specificFilePath = inputInfor+"/"+files[i];
		 std::cout<<"\nthe specificFilePath for the sending file:"<<specificFilePath;
		 try{Sender sdr(1,specificFilePath,choose);
	  DemoThread t1(sdr,IP,remotPort,srcPortNum);
	  t1.start(); t1.join(); Receiver rcvr;
	  rcvr.start(srcPortNum); 
	  return rcvr.getState();
		 }
	  catch(std::exception& ex){ sout << "\n\n  " << ex.what();ret = 1; }
	  catch(...) {sout << "\n\n  something bad happend to a sender";ret = 1; }
	  std::cout << "\n\n  That's all Folks!\n\n";		 }
	 }if(style == 2){
		 try{Sender sdr(1,inputInfor,choose);
	  DemoThread t1(sdr,IP,remotPort,srcPortNum);
	  t1.start();t1.join(); Receiver rcvr;
	  rcvr.start(srcPortNum); }
	  catch(std::exception& ex){  sout << "\n\n  " << ex.what(); ret = 1; }
	  catch(...) {sout << "\n\n  something bad happend to a sender"; ret = 1; }
	  std::cout << "\n\n  That's all Folks!\n\n";	}
 }
}
//};
#endif