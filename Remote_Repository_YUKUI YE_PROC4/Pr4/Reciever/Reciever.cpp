/////////////////////////////////////////////////////////////////////
// Reciever.cpp - Demonstration of socket reciever with            //
//                concurrent clients                               //
// Language:    Visual C++ 2012                                    //
// Platform:    MACPRO, Virtual Machine Windows 8                  //
// Application: Help for CSE687 - OOD, Pr#1, Spring 2013           //
// Author:      YUKUI YE,            Syracuse University           //
//              (315) 751-0156, yye@syr.edu                        //
/////////////////////////////////////////////////////////////////////

#include "../Sockets/Sockets.h"
#include "../Threads/Threads.h"
#include "../Threads/Locks.h"
#include "../BlockingQueue/BlockingQueue.h"
#include "../Xml/XmlWriter.h"
#include "../Xml/XmlReader.h"
#include "AcknowledgementMsg.h"
#include "../FileSystem/FileSystem.h"
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include "../FileSystem/FileSystem.h"
#include  "../Base64/Base64.h"
#include "../Base64/Base64.cpp"
#include "../FileSystem/FileManager.h"

// SendThread thread

class SendThread : public threadBase
{
public:
  SendThread(Socket s, BlockingQueue<std::string>& q) : s_(s), q_(q) {};
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
  };
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

class Sender
{
public:
  Sender(std::string ret) : retmsg(ret) { myCount = ++count; }
  int id() { return myCount; }
  void start(std::string ip, int port){
    sout << locker << "\n Sender #" << id() << " started" << unlocker;
    pSt = new SendThread(s_, q_);
    pSt->start();
    if(!s_.connect(ip, port)){
      sout << locker << "\n  couldn't connect to " << ip << ":" << port << "\n\n" << unlocker;
      delete pSt;
      return;
    }
    else{
      std::string logMsg = "\n  connected to " + ip + ":" + ToString(port);
      doLog(logMsg.c_str());
    }
    doLog("starting Sending back message");
    //for(int i=0; i<numMsgs_; ++i){
      doLog("Got Your Message");
	  sout << locker << "\n  " << retmsg.c_str() << unlocker;
      q_.enQ(retmsg);
  //  }
    q_.enQ("stop");
	retmsg = "sender#" + ToString(id()) + ": stop";
	sout << "\n  " + retmsg;
    pSt->join();
   // delete pSt;
  }
private:
  Socket s_;
  BlockingQueue<std::string> q_;
  SendThread* pSt;
  static int count;
  int myCount;
 // int numMsgs_;
  std::string retmsg;
};

int Sender::count = 0;

// for sending back message to notify the sender we got the message
class AcknowledgementThread : public threadBase
{
public:
	AcknowledgementThread(Sender sndr,   BlockingQueue<AcknowlegementMsg>& acknowledgementMsgQ) : 
		sndr_(sndr),ackmsgQ(acknowledgementMsgQ){};
private:
  void run()
  {
	  AcknowlegementMsg ackMsg = ackmsgQ.deQ();
	  std::string IP = ackMsg.IP;
	  int srcPort = ackMsg.srcPort;
	  sndr_.start(IP, srcPort);
  };
  Sender sndr_;
  BlockingQueue<AcknowlegementMsg>& ackmsgQ;
};

///////////////////////////////////////////////////
// ClientHandlerThread thread

class ClientHandlerThread : public tthreadBase
{
public:
  ClientHandlerThread(Socket s, BlockingQueue<std::string>& q, bool sflag) :stopFlag(sflag), s_(s), q_(q) {}
private:
	bool stopFlag;
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
	stopFlag=true;
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
      ClientHandlerThread* pCh = new ClientHandlerThread(s, q_,false);
      pCh->start();
    }
  }
  bool stop_;
  BlockingQueue<std::string>& q_;
  SocketListener listener_;
};
////////////////////////////////////////////////////////////////////////////////
//checkFileAccessValidation: pass in the upcoming fileName
//check whether file already there or not
//1)if file already there, skip create new folder,then check its owner name;   
//2)file not there, store it.
//////////////////////////////////////
bool checkFileAccessValidation(std::string msg)
{
	bool retmsg = true;size_t i=0; 
	FileManager filemgr;
	std::vector<std::string> fileList = filemgr.getFileList("..\\Reciever\\storedPackage");
	while(i < fileList.size()-1){
		if(msg == fileList[i]){	 //if found the file in current repository, then return false
			retmsg = false;
			break;
		} i=i+1;
	}
	return retmsg;
}
/////////////////////////////////////////////////////////////////////////////////////////////
//if file already in repository, read the file's xmlMetaData file
//in order to Compare the upcoming file's username with the file's owner name
//if the username who want to send the file is equal to the file's owner
//11)If the sendingFileUsername is not the owner, then denied it
//12)If it is the owner, means modify the file. sendbackmsg" successfully accessed,same owner"
/////////////////////////////////////////////////////////////
std::string checkXMLMetaData(std::string filename,std::string username)
{
    std::string retmsg;
	std::string xmlfilename = filename+"_xmlMetaData";
	std::ifstream in("..//Reciever//storedPackage//"+filename+"//"+xmlfilename+".xml");
	std::string inmsg,line;
	if(in.is_open()){
		while(getline(in,line)){
			inmsg += line;
		}
		in.close();
	}  
	XmlReader  rdlocalXML(inmsg);
	rdlocalXML.reset();
	while(rdlocalXML.next()){
		if(rdlocalXML.tag() == "owner"){
			if( username == rdlocalXML.body().c_str()){
				retmsg = "successfully accessed the file again, same owner";break;
			}
			else{
				retmsg = "denied. user is different from file owner";break;
			}
		}
	}
	return retmsg;
}
/////////////////////////////////////////////////////////
//chunk textMessage and write to xmlfile
//the coming message formate: 
//username*versionNumber*dependency$goingtosendfile'sName
////////////////////////////////////////////////////////
std::string writeToXML(std::string msg)
{
	bool v; std::string retmsg;
	XmlWriter myxml;
	std::string msgOwner,msgNumber,msgDependency;
	int pos1,pos2,pos3;
    pos3 = msg.find_last_of("$");
	std::string sendingFileName = msg.substr(pos3+1);
	pos1 = msg.find_first_of("*");
	msgOwner = msg.substr(0,pos1);
	msg = msg.substr(pos1+1);
	pos2 = msg.find_last_of("*");
	msgNumber = msg.substr(0,pos2);
	msgDependency = msg.substr(pos2+1);
	int pos = sendingFileName.find(".");
	std::string justname= sendingFileName.substr(0,pos);
	bool check = checkFileAccessValidation(sendingFileName);
   if(check){ //file not in repository	
	std::string folderPath = "..//Reciever//storedPackage//"+justname;
	v = FileSystem::Directory::create(folderPath);

	myxml.addDeclaration();
	myxml.addComment("XML MetaData for file "+sendingFileName);
	myxml.start("root");
	myxml.element("owner",msgOwner);
	myxml.element("versionNumber",msgNumber);
	myxml.element("dependency",msgDependency);
	myxml.end();
	//if(v){
	std::ofstream out(folderPath+"//"+justname+"_xmlMetadata.xml");
	if(!out.good()){  throw std::invalid_argument(std::string("can't open file ") + sendingFileName);}
	out << myxml.xml().c_str();
	out.close();
	retmsg = "successfully stored xmlMetadata for file "+sendingFileName;
   }else{//file is already in the repository 
	  retmsg = checkXMLMetaData(justname,msgOwner);
   }
	return retmsg;
}
///////////////////////////////////////////
//Check user account validation : 
//read local usercheckin xml file and compare it with the upcoming message
//the coming messge formate : username*password
/////////////////////////////////////
std::string validationAccount(std::string msg)
{
	int pos; std::string retmsg;
	pos = msg.find("*");
	std::string comingUsername = msg.substr(0,pos);
	std::string comingPassword = msg.substr(pos+1);
	std::ifstream in("checkIn.xml");
	std::string inmsg,line;
	if(in.is_open()){
		while(getline(in,line)){
			inmsg += line;
		}
		in.close();
	}  
	bool validname= false,validpwd= false;
	XmlReader  rdlocalXML(inmsg);
	rdlocalXML.reset();
	while(rdlocalXML.next()){
		if(rdlocalXML.tag() == "username"){
			std::string namefromXML =  rdlocalXML.body().c_str();
			if(comingUsername == namefromXML)    validname = true;
			else{ retmsg = "Invalid User name, denied";}
		}
		if(rdlocalXML.tag() == "password"){
				std::string pwdfromXML = rdlocalXML.body().c_str();
				if(comingPassword == pwdfromXML)  validpwd = true;
				else{ retmsg = "wrong password,try again";}
		}
		if(validname && validpwd){retmsg = "permitted";break;}
	}		
	return retmsg;
}
/////////////////////////////////////////////////////////////////////////////
//distinguist different type of message
//messageType: login*username*password 
//         or  xmlMetaData*username*versionNumber*dependency$filename
////////////////////////////////////////////////////////////////////////
std::string comingmsgOperation(std::string msg)
{
	std::string retmsg ;
	int pos = msg.find_first_of("*");
	std::string msgType = msg.substr(0,pos);
	std::string msgLeft = msg.substr(pos+1);
	if(msgType == "login"){
		retmsg = validationAccount(msgLeft);
	}
	if(msgType == "xmlMetaData"){
		retmsg = writeToXML(msgLeft);
	}
	return retmsg;
}
/////////////////////////////////////////////////////////////////////////////////////////////
//write the coming file into local file folder.
//Because In my design,I first send the file's xmlMetaData information,at the same time,
//check whether the upcoming file is already stored in repository or not,
//create a new folder(file's name) as well as the "filename_xmlMetaData.xml" for the upcoming file.
//thus, writeToFile is the third step,the folder has already been created in the previous step.
/////////////////////////////////////////////////////////////////////////////////////
void writeToFile(std::string msg,std::string name)
{
	bool v;
	int pos = name.find(".");
	std::string n= name.substr(0,pos);
	std::string path = "..//Reciever//storedPackage//"+n;
	/*v = FileSystem::Directory::create(path);
	if(v){*/
	std::ofstream out(path+"//"+name);
	if(!out.good()){  throw std::invalid_argument(std::string("can't open file ") + name);}
	out <<msg;
	out.close();
	/*}else{ std::cout<<"fail to create folder";}*/
}
//////////////////////////////////////////////
class Receiver
{
public:
	Receiver(BlockingQueue<AcknowlegementMsg>& acknowledgementMsgQ):_ackMsgQ(acknowledgementMsgQ){};
  void start(int port){
    sout << "\n\nReciever started\n";
    pLt = new ListenThread(port, q_);
    try{
      pLt->start();
      std::string temp;std::string msg;
      int count = 0; std::string retmsg;
	  AcknowlegementMsg ackMsg;
      do{	
        temp = q_.deQ(); //extract the xml formate string to get the IP address
		XmlReader  rdmsg(temp);
		rdmsg.reset();
		if(temp != "stop"){
		while(rdmsg.next()){
			if(rdmsg.tag() == "IP")
				ackMsg.IP = rdmsg.body().c_str();
			if(rdmsg.tag() == "sourcePort")
				ackMsg.srcPort = atoi(rdmsg.body().c_str());
			if(rdmsg.tag() == "type")
				ackMsg.type = rdmsg.body().c_str();
			if(ackMsg.type == "text"){
					if(rdmsg.tag() == "body"){
						ackMsg.msg = rdmsg.body().c_str();
						msg = ackMsg.msg;
						_ackMsgQ.enQ(ackMsg);
						retmsg = comingmsgOperation(ackMsg.msg);
						ackMsg.retmsg = retmsg; 
						_ackMsgQ.enQ(ackMsg);
					}
			}
			if(rdmsg.tag() =="fileName") ackMsg.fileName = rdmsg.body().c_str();
			if(ackMsg.type =="file"){
				if(rdmsg.tag() == "body" ){
				    std::string inmsg = rdmsg.body(); //decode the body based on the blockSize
					int num = inmsg.size()/1024;
					for(size_t i=0;i<=num;i++){ //unchunk the file
						std::string tmp = inmsg.substr(i*1024,1024+1024*i);
						std::vector<char> vdecoded = Base64::decode(tmp);
		                std::string decodedMsg(vdecoded.begin(), vdecoded.end());
						ackMsg.msg += decodedMsg;
						ackMsg.retmsg = "fileRecieved";
					    writeToFile(ackMsg.msg,ackMsg.fileName);}
				}	_ackMsgQ.enQ(ackMsg);
				msg=ackMsg.msg;
				doLog((std::string("fetching message ") + msg).c_str());}}
		}if(temp == "stop") {
			msg = "stop";
			Sender sndr1(ackMsg.retmsg);
	        AcknowledgementThread t1(sndr1,_ackMsgQ);
            t1.start();
	        t1.join();
		}doLog((std::string("fetching message ") + msg).c_str());
        sout << "\n  Recieved from " << msg.c_str();
      } while(true);
    }catch(std::exception& ex){
      delete pLt;
	}catch(...){
      delete pLt;
    }
  }
private:
  BlockingQueue<std::string> q_;
  BlockingQueue<AcknowlegementMsg> &_ackMsgQ;
  ListenThread* pLt;
};

///////////////////////////receiver main function////////////////////////
int main()
{
  int ret = 0;
  BlockingQueue<AcknowlegementMsg> ackMsgQ;
  AcknowlegementMsg ackmsg;
  try
  {
    Receiver rcvr(ackMsgQ);
    rcvr.start(8080);
  }
  catch(std::exception& ex)
  {
    std::cout << "\n\n  " << ex.what();
    ret = 1;
  }
  catch(...)
  {
    sout << "\n  something bad happened";
    ret = 1;
  }
  sout << "\n\n";
  return ret;
}