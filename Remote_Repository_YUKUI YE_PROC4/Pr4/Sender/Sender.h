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
  Sender(std::string inputmsg,int choose) : inputInfor(inputmsg),fileType(choose) { myCount = ++count; }
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
   //   for(int i=0; i<numMsgs_; ++i) {
      doLog("sending message");
	  std::string temp = "<msg>"; 
      temp += std::string("<IP>") + ip + "</IP>"; //I assume the sourceIP is the same as ReceiverIPaddress
	  temp += std::string("<sourcePort>") + ToString(sourcePort) + "</sourcePort>";
	  temp += std::string("<type>") + "text" + "</type>";
      temp += std::string("<body>") + inputInfor + "</body></msg>";
      sout << locker << "\n  " << temp.c_str() << unlocker;
      q_.enQ(temp);
	}
	if(fileType==2){
	//	for(int i=0; i<numMsgs_; ++i) {
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
	  q_.enQ(temp1);//
	} 
      q_.enQ("stop");
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
 // int numMsgs_;
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
public:
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
		retmsg += msg;
        doLog((std::string("fetching message ") + msg).c_str());
        sout << "\n  Recieved from: " << msg.c_str();
		if(msg == "stop"){
			break;
		}
      } while(true);
    }
    catch(std::exception& ex)
    {
      delete pLt;
    }
    catch(...)
    {
      delete pLt;
    }
  }
  std::string getretmsg()
  {
	  return retmsg;
  }
private:
  std::string retmsg;
  BlockingQueue<std::string> q_;
  ListenThread* pLt;
};

  ////////////////////////////
  //forSendingMsgType: if send message choose = 1; if send file choose = 2;
  //loginWindow :  username*password&IP&SrcPort&RmtPort
  //sendXMLmetaDataWindow: username*versionNumber*dependency$filename&IP&SrcPort&RmtPort
  //sendFileWindow:  filename .cpp or .h or .txt
  //////////////////////////////////////
class myChannel
{
public:
	std::string channelStart(std::string msg, int choose)
	{
		std::string retmsg,msginfor,msg1,msg2,msg3;
		int pos1,pos2,pos3;
		if(choose == 1){//send textmessage
			pos1 = msg.find_first_of("&"); //separate msginformation with computerInformation
			msg1 = msg.substr(0,pos1); //contain message information;
			msg2 = msg.substr(pos1+1); //contain ip,srcport,rmtport information
			pos2 = msg2.find_first_of("&");
			std::string ip = msg2.substr(0,pos2);
			msg3 = msg2.substr(pos2+1); // contain srcport and rmtport
			pos3 = msg3.find("&");
			std::string srcPort = msg3.substr(0,pos3);
			std::string rmtPort = msg3.substr(pos3+1);
			int spt = atoi(srcPort.c_str());
			int rpt = atoi(rmtPort.c_str());
			if(msg.find("$") < msg.length()){ //send xmlMetaData information
				msginfor = "xmlMetaData*"+msg1;
				retmsg = getInforFromCommand(1,msginfor,spt,rpt,ip);
			}else // send login information
			{
				msginfor = "login*"+msg1;
				retmsg =  getInforFromCommand(1,msginfor,spt,rpt,ip);		 
			}
		}
		if(choose == 2)//send file
			retmsg = getInforFromCommand(2,msg,8090,8080,"127.0.0.1");
		return retmsg;
	}
///////////////////////////////////////////////
////int choose:"I messageType: 1:SendText; 2:SendFile"
////std::string inputInfor : textMessage or filename
////////////////////////////
std::string	getInforFromCommand(int choose,std::string infor,int port,int rmtPort,std::string ip)
{
	static int i =0;
	i++;
	port += i;
	std::string myretmsg;
  int srcPortNum = port, remotPort = rmtPort; int ret = 0;
  std::string inputInfor = infor,IP = ip;
 if(choose==1){
	  try{Sender sdr(inputInfor,choose);
	  DemoThread t1(sdr,IP,remotPort,srcPortNum);
	  t1.start(); t1.join();
      Receiver rcvr; 
	  rcvr.start(srcPortNum);
	  myretmsg = rcvr.getretmsg();
	  } catch(std::exception& ex){  sout << "\n\n  " << ex.what();ret = 1; }
	  catch(...) {sout << "\n\n  something bad happend to a sender"; ret = 1; }
	  std::cout << "\n\n  That's all Folks!\n\n";
 } if(choose==2){  
		 try{Sender sdr(inputInfor,choose);
	  DemoThread t1(sdr,IP,remotPort,srcPortNum);
	  t1.start(); t1.join(); 
	  Receiver rcvr;
	  rcvr.start(srcPortNum); 
	  myretmsg = rcvr.getretmsg();
		 } catch(std::exception& ex){ sout << "\n\n  " << ex.what();ret = 1; }
	  catch(...) {sout << "\n\n  something bad happend to a sender";ret = 1; }
	  std::cout << "\n\n  That's all Folks!\n\n";		
    }
 return myretmsg;
 }
};
#endif